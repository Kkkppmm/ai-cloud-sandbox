#include "test_framework.hpp"
#include "storage/storage_manager.hpp"

TEST(storage_atomic_write_and_quota) {
  ai_cloud::storage::StorageManager mgr("/tmp/ai-cloud-tests-storage");
  auto ws = mgr.workspace_for("agent-a", 1);
  REQUIRE(ws != nullptr);
  std::string err;
  REQUIRE(ws->atomic_write("out.txt", "hello", &err));
  REQUIRE(ws->within_quota(0));
}
