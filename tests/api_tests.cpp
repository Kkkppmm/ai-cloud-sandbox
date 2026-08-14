#include "test_framework.hpp"
#include "api/request_handler.hpp"

TEST(api_health_endpoint) {
  ai_cloud::api::RequestHandler h;
  auto res = h.handle({"/health", "GET", {}, ""});
  REQUIRE(res.status == 200);
  REQUIRE(res.body == "ok");
}
