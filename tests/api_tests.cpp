#include "test_framework.hpp"
#include "api/http_server.hpp"
#include "api/request_handler.hpp"
#include "api/websocket_handler.hpp"
#include <arpa/inet.h>
#include <chrono>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

TEST(api_health_endpoint) {
  ai_cloud::api::RequestHandler h;
  auto res = h.handle({"/health", "GET", {}, ""});
  REQUIRE(res.status == 200);
  REQUIRE(res.body == "ok");
}

TEST(websocket_payload_is_escaped) {
  ai_cloud::api::WebSocketHandler w;
  auto out = w.format_status_event("a\"1", "ok\\n");
  REQUIRE(out.find("\\\"") != std::string::npos);
  REQUIRE(out.find("\\\\") != std::string::npos);
}

TEST(http_server_serves_health_endpoint) {
  ai_cloud::api::HttpServer server(ai_cloud::api::RequestHandler{}, "127.0.0.1", 18081);
  std::string err;
  REQUIRE(server.start(&err));

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  REQUIRE(fd >= 0);
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(18081);
  REQUIRE(::inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) == 1);
  REQUIRE(::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0);

  const std::string req = "GET /health HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
  REQUIRE(::send(fd, req.data(), req.size(), 0) >= 0);

  char buf[512]{};
  const auto n = ::recv(fd, buf, sizeof(buf), 0);
  REQUIRE(n > 0);
  const std::string response(buf, static_cast<std::size_t>(n));
  REQUIRE(response.find("HTTP/1.1 200 OK") != std::string::npos);
  REQUIRE(response.find("ok") != std::string::npos);

  ::close(fd);
  server.stop();
}
