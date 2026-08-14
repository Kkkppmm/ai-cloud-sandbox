#pragma once
#include "tool_registry.hpp"
#include <memory>
#include <set>

namespace ai_cloud::tools {

class ToolExecutor {
 public:
  explicit ToolExecutor(std::shared_ptr<ToolRegistry> registry);
  ToolResponse execute(const std::string& tool_name, const ToolRequest& request,
                       const std::set<Permission>& granted_permissions) const;

 private:
  std::shared_ptr<ToolRegistry> registry_;
};

} // namespace ai_cloud::tools
