#pragma once
#include "tool.hpp"
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace ai_cloud::tools {

class ToolRegistry {
 public:
  bool register_tool(std::shared_ptr<Tool> tool);
  std::shared_ptr<Tool> get(const std::string& name) const;
  std::vector<std::string> list() const;

 private:
  mutable std::mutex mu_;
  std::unordered_map<std::string, std::shared_ptr<Tool>> tools_;
};

} // namespace ai_cloud::tools
