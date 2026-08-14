#pragma once
#include <string>

namespace ai_cloud::api {

class WebSocketHandler {
 public:
  std::string format_status_event(const std::string& agent_id, const std::string& state) const;
};

} // namespace ai_cloud::api
