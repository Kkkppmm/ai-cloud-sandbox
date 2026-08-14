#include "http_server.hpp"
#include <arpa/inet.h>
#include <algorithm>
#include <cerrno>
#include <cctype>
#include <cstring>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

namespace ai_cloud::api {
namespace {

std::string status_text(int status) {
  switch (status) {
    case 200:
      return "OK";
    case 404:
      return "Not Found";
    case 500:
      return "Internal Server Error";
    default:
      return "Unknown";
  }
}

std::string trim(std::string value) {
  value.erase(value.begin(),
              std::find_if(value.begin(), value.end(), [](unsigned char ch) { return !std::isspace(ch); }));
  value.erase(std::find_if(value.rbegin(), value.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(),
              value.end());
  return value;
}

Request parse_request(const std::string& raw) {
  Request req;
  std::istringstream stream(raw);
  std::string line;
  if (!std::getline(stream, line)) return req;
  if (!line.empty() && line.back() == '\r') line.pop_back();

  std::istringstream first_line(line);
  std::string version;
  first_line >> req.method >> req.path >> version;

  while (std::getline(stream, line)) {
    if (line == "\r" || line.empty()) break;
    if (!line.empty() && line.back() == '\r') line.pop_back();
    const auto colon = line.find(':');
    if (colon == std::string::npos) continue;
    req.headers[trim(line.substr(0, colon))] = trim(line.substr(colon + 1));
  }
  return req;
}

std::string build_response(const Response& res) {
  std::ostringstream out;
  out << "HTTP/1.1 " << res.status << " " << status_text(res.status) << "\r\n";
  out << "Content-Type: text/plain\r\n";
  out << "Content-Length: " << res.body.size() << "\r\n";
  out << "Connection: close\r\n\r\n";
  out << res.body;
  return out.str();
}

} // namespace

HttpServer::HttpServer(RequestHandler handler, std::string bind_address, int port)
    : handler_(std::move(handler)), bind_address_(std::move(bind_address)), port_(port) {}

HttpServer::~HttpServer() { stop(); }

bool HttpServer::start(std::string* error) {
  if (running_) return true;

  server_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd_ < 0) {
    if (error) *error = std::string("socket failed: ") + std::strerror(errno);
    return false;
  }

  int opt = 1;
  if (::setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
    if (error) *error = std::string("setsockopt failed: ") + std::strerror(errno);
    ::close(server_fd_);
    server_fd_ = -1;
    return false;
  }

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(static_cast<uint16_t>(port_));
  if (::inet_pton(AF_INET, bind_address_.c_str(), &addr.sin_addr) != 1) {
    if (error) *error = "invalid bind address: " + bind_address_;
    ::close(server_fd_);
    server_fd_ = -1;
    return false;
  }

  if (::bind(server_fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
    if (error) *error = std::string("bind failed: ") + std::strerror(errno);
    ::close(server_fd_);
    server_fd_ = -1;
    return false;
  }
  if (::listen(server_fd_, 64) != 0) {
    if (error) *error = std::string("listen failed: ") + std::strerror(errno);
    ::close(server_fd_);
    server_fd_ = -1;
    return false;
  }

  running_ = true;
  const auto worker_count = std::max(2u, std::thread::hardware_concurrency());
  workers_.reserve(worker_count);
  for (unsigned i = 0; i < worker_count; ++i) {
    workers_.emplace_back(&HttpServer::worker_loop, this);
  }
  server_thread_ = std::thread(&HttpServer::serve_loop, this);
  return true;
}

void HttpServer::serve_loop() {
  while (running_) {
    const int client_fd = ::accept(server_fd_, nullptr, nullptr);
    if (client_fd < 0) {
      if (!running_) break;
      if (errno == EINTR) continue;
      continue;
    }
    {
      std::lock_guard<std::mutex> lock(clients_mu_);
      pending_clients_.push_back(client_fd);
    }
    clients_cv_.notify_one();
  }
}

void HttpServer::worker_loop() {
  while (true) {
    int client_fd = -1;
    {
      std::unique_lock<std::mutex> lock(clients_mu_);
      clients_cv_.wait(lock, [this] { return !running_ || !pending_clients_.empty(); });
      if (!running_ && pending_clients_.empty()) return;
      client_fd = pending_clients_.front();
      pending_clients_.pop_front();
    }
    handle_connection(client_fd);
    ::close(client_fd);
  }
}

void HttpServer::handle_connection(int client_fd) const {
  timeval timeout{};
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;
  (void)::setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  std::string request_data;
  char buffer[4096];
  while (request_data.find("\r\n\r\n") == std::string::npos) {
    const ssize_t n = ::recv(client_fd, buffer, sizeof(buffer), 0);
    if (n <= 0) return;
    request_data.append(buffer, static_cast<std::size_t>(n));
    if (request_data.size() > 64 * 1024) return;
  }

  const Request req = parse_request(request_data);
  const Response res = handler_.handle(req);
  const std::string payload = build_response(res);
  (void)::send(client_fd, payload.data(), payload.size(), 0);
}

void HttpServer::stop() {
  if (!running_) return;
  running_ = false;
  clients_cv_.notify_all();
  if (server_fd_ >= 0) {
    ::shutdown(server_fd_, SHUT_RDWR);
    ::close(server_fd_);
    server_fd_ = -1;
  }
  if (server_thread_.joinable()) server_thread_.join();
  for (auto& worker : workers_) {
    if (worker.joinable()) worker.join();
  }
  workers_.clear();
  std::lock_guard<std::mutex> lock(clients_mu_);
  while (!pending_clients_.empty()) {
    ::close(pending_clients_.front());
    pending_clients_.pop_front();
  }
}

} // namespace ai_cloud::api
