#include "test_framework.hpp"
#include "api/request_handler.hpp"
#include "api/websocket_handler.hpp"

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
