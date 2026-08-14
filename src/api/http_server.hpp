#pragma once
#include "request_handler.hpp"
#include <atomic>
#include <string>
#include <thread>

namespace ai_cloud::api {

class HttpServer {
 public:
  explicit HttpServer(RequestHandler handler, std::string bind_address = "127.0.0.1", int port = 8080);
  ~HttpServer();
  bool start(std::string* error = nullptr);
  void stop();
  bool running() const noexcept { return running_; }

 private:
  void serve_loop();
  void handle_connection(int client_fd) const;

  RequestHandler handler_;
  std::string bind_address_;
  int port_;
  int server_fd_{-1};
  std::thread server_thread_{};
  std::atomic<bool> running_{false};
};

} // namespace ai_cloud::api
