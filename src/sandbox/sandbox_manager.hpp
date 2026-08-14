#pragma once
#include "core/config.hpp"
#include "cgroup_manager.hpp"
#include "namespace_manager.hpp"
#include "resource_limiter.hpp"
#include "seccomp_filter.hpp"
#include <string>

namespace ai_cloud::sandbox {

class SandboxManager {
 public:
  bool prepare_agent(const std::string& agent_id, const core::DaemonConfig& cfg, std::string* error) const;

 private:
  NamespaceManager ns_{};
  CgroupManager cgroup_{};
  ResourceLimiter limiter_{};
  SeccompFilter seccomp_{};
};

} // namespace ai_cloud::sandbox
