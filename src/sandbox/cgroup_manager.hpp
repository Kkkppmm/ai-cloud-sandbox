#pragma once
#include "core/config.hpp"
#include <string>

namespace ai_cloud::sandbox {

class CgroupManager {
 public:
  explicit CgroupManager(std::string root = "/sys/fs/cgroup/ai-cloud-sandbox");
  bool apply_limits(const std::string& agent_id, const core::ResourceConfig& cfg, std::string* error) const;

 private:
  std::string root_;
};

} // namespace ai_cloud::sandbox
