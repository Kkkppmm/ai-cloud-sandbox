add_library(ai_cloud_sandbox_lib
  main.cpp
  core/agent.cpp
  core/agent_manager.cpp
  core/config.cpp
  core/event_loop.cpp
  sandbox/sandbox_manager.cpp
  sandbox/namespace_manager.cpp
  sandbox/cgroup_manager.cpp
  sandbox/seccomp_filter.cpp
  sandbox/resource_limiter.cpp
  tools/tool.cpp
  tools/tool_registry.cpp
  tools/tool_executor.cpp
  tools/builtin_tools.cpp
  storage/storage_manager.cpp
  storage/filesystem_isolation.cpp
  storage/quota_manager.cpp
  storage/workspace.cpp
  api/http_server.cpp
  api/grpc_server.cpp
  api/websocket_handler.cpp
  api/request_handler.cpp
  api/routes.cpp
  security/auth.cpp
  security/audit_log.cpp
  security/crypto_utils.cpp
  monitoring/metrics.cpp
  monitoring/health_check.cpp
  monitoring/resource_monitor.cpp
  utils/logger.cpp
  utils/json_utils.cpp
  utils/process_utils.cpp
  utils/string_utils.cpp
  utils/error.cpp
)

target_include_directories(ai_cloud_sandbox_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Threads REQUIRED)
target_link_libraries(ai_cloud_sandbox_lib PUBLIC Threads::Threads)

add_executable(ai-cloud-daemon daemon_main.cpp)
target_link_libraries(ai-cloud-daemon PRIVATE ai_cloud_sandbox_lib)
