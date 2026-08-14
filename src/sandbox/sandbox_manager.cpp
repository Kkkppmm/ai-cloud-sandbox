#include "sandbox_manager.hpp"

namespace ai_cloud::sandbox {

bool SandboxManager::prepare_agent(const std::string& agent_id, const core::DaemonConfig& cfg, std::string* error) const {
  if (!limiter_.set_memory_limit_mb(cfg.resources.memory_limit_mb)) {
    if (error) *error = "failed to apply rlimit";
    return false;
  }
  if (!ns_.isolate_process(cfg.security.enable_network, error)) return false;
  if (!cgroup_.apply_limits(agent_id, cfg.resources, cfg.security.cgroup_policy_path, error)) return false;
  if (cfg.security.enable_seccomp && !seccomp_.apply_policy(cfg.security.seccomp_policy_path, error)) return false;
  return true;
}

} // namespace ai_cloud::sandbox
