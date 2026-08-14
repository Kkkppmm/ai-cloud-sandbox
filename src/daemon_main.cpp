#include "core/agent_manager.hpp"
#include "core/config.hpp"
#include "tools/builtin_tools.hpp"
#include "tools/tool_executor.hpp"
#include "tools/tool_registry.hpp"
#include "utils/logger.hpp"
#include <iostream>

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  ai_cloud::utils::Logger::instance().info("starting ai-cloud-daemon");

  auto registry = std::make_shared<ai_cloud::tools::ToolRegistry>();
  ai_cloud::tools::register_builtin_tools(*registry);
  ai_cloud::tools::ToolExecutor executor(registry);

  ai_cloud::core::AgentManager manager(4, executor);
  manager.start();
  manager.stop();

  std::cout << "ai-cloud-daemon initialized" << std::endl;
  return 0;
}
