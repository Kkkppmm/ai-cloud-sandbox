#include "websocket_handler.hpp"
#include "utils/json_utils.hpp"

namespace ai_cloud::api {

std::string WebSocketHandler::format_status_event(const std::string& agent_id, const std::string& state) const {
  return "{\"agent_id\":\"" + ai_cloud::utils::json_escape(agent_id) + "\",\"state\":\"" +
         ai_cloud::utils::json_escape(state) + "\"}";
}

} // namespace ai_cloud::api
