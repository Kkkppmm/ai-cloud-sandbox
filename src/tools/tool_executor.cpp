#include "tool_executor.hpp"

namespace ai_cloud::tools {

ToolExecutor::ToolExecutor(std::shared_ptr<ToolRegistry> registry) : registry_(std::move(registry)) {}

ToolResponse ToolExecutor::execute(const std::string& tool_name, const ToolRequest& request,
                                   const std::set<Permission>& granted_permissions) const {
  if (!registry_) return {false, "", "tool registry unavailable"};
  auto tool = registry_->get(tool_name);
  if (!tool) return {false, "", "tool not found"};
  for (Permission p : tool->required_permissions()) {
    if (granted_permissions.count(p) == 0) {
      return {false, "", "permission denied"};
    }
  }
  return tool->execute(request);
}

} // namespace ai_cloud::tools
