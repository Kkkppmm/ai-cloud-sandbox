#include "tool_registry.hpp"

namespace ai_cloud::tools {

bool ToolRegistry::register_tool(std::shared_ptr<Tool> tool) {
  if (!tool) return false;
  std::lock_guard<std::mutex> lock(mu_);
  return tools_.emplace(tool->name(), std::move(tool)).second;
}

std::shared_ptr<Tool> ToolRegistry::get(const std::string& name) const {
  std::lock_guard<std::mutex> lock(mu_);
  auto it = tools_.find(name);
  return it == tools_.end() ? nullptr : it->second;
}

std::vector<std::string> ToolRegistry::list() const {
  std::lock_guard<std::mutex> lock(mu_);
  std::vector<std::string> out;
  out.reserve(tools_.size());
  for (const auto& kv : tools_) out.push_back(kv.first);
  return out;
}

} // namespace ai_cloud::tools
