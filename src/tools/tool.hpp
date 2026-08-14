#pragma once
#include <set>
#include <string>
#include <vector>

namespace ai_cloud::tools {

enum class Permission { ReadFiles, WriteFiles, ExecuteShell, ListDirectory };

struct ToolRequest {
  std::vector<std::string> args;
};

struct ToolResponse {
  bool ok{false};
  std::string output;
  std::string error;
};

class Tool {
 public:
  virtual ~Tool() = default;
  virtual std::string name() const = 0;
  virtual std::set<Permission> required_permissions() const = 0;
  virtual ToolResponse execute(const ToolRequest& request) = 0;
};

} // namespace ai_cloud::tools
