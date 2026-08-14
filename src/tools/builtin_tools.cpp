#include "builtin_tools.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace ai_cloud::tools {

namespace {

class WriteFileTool final : public Tool {
 public:
  std::string name() const override { return "write_file"; }
  std::set<Permission> required_permissions() const override { return {Permission::WriteFiles}; }
  ToolResponse execute(const ToolRequest& request) override {
    if (request.args.size() < 2) return {false, "", "usage: write_file <path> <content>"};
    std::ofstream out(request.args[0], std::ios::binary | std::ios::trunc);
    if (!out.good()) return {false, "", "cannot open file"};
    out << request.args[1];
    return {true, "ok", ""};
  }
};

class ReadFileTool final : public Tool {
 public:
  std::string name() const override { return "read_file"; }
  std::set<Permission> required_permissions() const override { return {Permission::ReadFiles}; }
  ToolResponse execute(const ToolRequest& request) override {
    if (request.args.empty()) return {false, "", "usage: read_file <path>"};
    std::ifstream in(request.args[0], std::ios::binary);
    if (!in.good()) return {false, "", "cannot open file"};
    std::ostringstream ss;
    ss << in.rdbuf();
    return {true, ss.str(), ""};
  }
};

class ListDirTool final : public Tool {
 public:
  std::string name() const override { return "list_dir"; }
  std::set<Permission> required_permissions() const override { return {Permission::ListDirectory}; }
  ToolResponse execute(const ToolRequest& request) override {
    const std::filesystem::path p = request.args.empty() ? "." : request.args[0];
    if (!std::filesystem::exists(p)) return {false, "", "path not found"};
    std::ostringstream ss;
    for (const auto& e : std::filesystem::directory_iterator(p)) {
      ss << e.path().filename().string() << "\n";
    }
    return {true, ss.str(), ""};
  }
};

} // namespace

void register_builtin_tools(ToolRegistry& registry) {
  (void)registry.register_tool(std::make_shared<WriteFileTool>());
  (void)registry.register_tool(std::make_shared<ReadFileTool>());
  (void)registry.register_tool(std::make_shared<ListDirTool>());
}

} // namespace ai_cloud::tools
