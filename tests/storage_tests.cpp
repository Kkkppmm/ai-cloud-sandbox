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

TEST(storage_rejects_path_traversal) {
  ai_cloud::storage::StorageManager mgr("/tmp/ai-cloud-tests-storage");
  auto ws = mgr.workspace_for("agent-b", 1);
  REQUIRE(ws != nullptr);
  std::string err;
  REQUIRE(!ws->atomic_write("../escape.txt", "nope", &err));
}

TEST(storage_allows_double_dot_in_filename_segment) {
  ai_cloud::storage::StorageManager mgr("/tmp/ai-cloud-tests-storage");
  auto ws = mgr.workspace_for("agent-c", 1);
  REQUIRE(ws != nullptr);
  std::string err;
  REQUIRE(ws->atomic_write("file..bak", "ok", &err));
}
