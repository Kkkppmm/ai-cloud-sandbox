#pragma once
#include <cstddef>
#include <string>

namespace ai_cloud::core {

struct ResourceConfig {
  std::size_t memory_limit_mb{256};
  int cpu_shares{256};
  std::size_t workspace_quota_mb{512};
};

struct SecurityConfig {
  bool enable_network{false};
  bool enable_seccomp{true};
};

struct ApiConfig {
  std::string bind_address{"127.0.0.1"};
  int port{8080};
};

struct DaemonConfig {
  ResourceConfig resources{};
  SecurityConfig security{};
  ApiConfig api{};
};

DaemonConfig load_config(const std::string& path);

} // namespace ai_cloud::core
