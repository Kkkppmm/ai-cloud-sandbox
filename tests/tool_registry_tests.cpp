#include "test_framework.hpp"
#include "tools/builtin_tools.hpp"
#include "tools/tool_executor.hpp"
#include <set>

TEST(tool_registry_enforces_permissions) {
  auto registry = std::make_shared<ai_cloud::tools::ToolRegistry>();
  ai_cloud::tools::register_builtin_tools(*registry);
  ai_cloud::tools::ToolExecutor exec(registry);
  auto denied = exec.execute("list_dir", {{"."}}, {});
  REQUIRE(!denied.ok);
  auto ok = exec.execute("list_dir", {{"."}}, {ai_cloud::tools::Permission::ListDirectory});
  REQUIRE(ok.ok);
}
