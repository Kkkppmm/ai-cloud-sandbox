#include "websocket_handler.hpp"

namespace ai_cloud::api {

std::string WebSocketHandler::format_status_event(const std::string& agent_id, const std::string& state) const {
  return "{\"agent_id\":\"" + agent_id + "\",\"state\":\"" + state + "\"}";
}

} // namespace ai_cloud::api
