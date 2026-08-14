#pragma once
#include "workspace.hpp"
#include <memory>
#include <string>

namespace ai_cloud::storage {

class StorageManager {
 public:
  explicit StorageManager(std::string root);
  std::shared_ptr<Workspace> workspace_for(const std::string& agent_id, std::size_t quota_mb) const;

 private:
  std::string root_;
};

} // namespace ai_cloud::storage
