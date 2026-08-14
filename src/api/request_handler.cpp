#include "request_handler.hpp"

namespace ai_cloud::api {

Response RequestHandler::handle(const Request& req) const {
  if (req.path == "/health") return {200, "ok"};
  return {404, "not found"};
}

} // namespace ai_cloud::api
