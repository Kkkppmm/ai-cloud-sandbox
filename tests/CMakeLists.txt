add_executable(ai-cloud-tests
  tool_registry_tests.cpp
  storage_tests.cpp
  api_tests.cpp
  integration_tests.cpp
)

target_link_libraries(ai-cloud-tests PRIVATE ai_cloud_sandbox_lib)
add_test(NAME ai-cloud-tests COMMAND ai-cloud-tests)
