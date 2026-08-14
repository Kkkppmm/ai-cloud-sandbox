#include "storage_manager.hpp"

namespace ai_cloud::storage {

StorageManager::StorageManager(std::string root) : root_(std::move(root)) {}

std::shared_ptr<Workspace> StorageManager::workspace_for(const std::string& agent_id, std::size_t quota_mb) const {
  auto ws = std::make_shared<Workspace>(root_ + "/" + agent_id, quota_mb * 1024 * 1024);
  if (!ws->ensure_exists()) return nullptr;
  return ws;
}

} // namespace ai_cloud::storage
