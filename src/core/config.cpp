#include "config.hpp"
#include <cctype>
#include <fstream>
#include <string>
#include <algorithm>

namespace {
std::string trim(std::string s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
  return s;
}

bool parse_bool(const std::string& value, bool fallback) {
  std::string v = value;
  std::transform(v.begin(), v.end(), v.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
  if (v == "true" || v == "1" || v == "yes" || v == "on") return true;
  if (v == "false" || v == "0" || v == "no" || v == "off") return false;
  return fallback;
}

std::string unquote(std::string value) {
  if (value.size() >= 2 && ((value.front() == '"' && value.back() == '"') || (value.front() == '\'' && value.back() == '\''))) {
    return value.substr(1, value.size() - 2);
  }
  return value;
}
} // namespace

namespace ai_cloud::core {

DaemonConfig load_config(const std::string& path) {
  DaemonConfig cfg;
  std::ifstream in(path);
  if (!in.good()) return cfg;

  std::string line;
  while (std::getline(in, line)) {
    auto pos = line.find(':');
    if (pos == std::string::npos) continue;
    std::string key = trim(line.substr(0, pos));
    std::string value = unquote(trim(line.substr(pos + 1)));
    if (key.find("port") != std::string::npos) {
      try {
        cfg.api.port = std::stoi(value);
      } catch (...) {
      }
    }
    if (key.find("memory_limit_mb") != std::string::npos) {
      try {
        cfg.resources.memory_limit_mb = static_cast<std::size_t>(std::stoull(value));
      } catch (...) {
      }
    }
    if (key.find("cpu_shares") != std::string::npos) {
      try {
        cfg.resources.cpu_shares = std::stoi(value);
      } catch (...) {
      }
    }
    if (key.find("workspace_quota_mb") != std::string::npos) {
      try {
        cfg.resources.workspace_quota_mb = static_cast<std::size_t>(std::stoull(value));
      } catch (...) {
      }
    }
    if (key.find("enable_network") != std::string::npos) cfg.security.enable_network = parse_bool(value, cfg.security.enable_network);
    if (key.find("enable_seccomp") != std::string::npos) cfg.security.enable_seccomp = parse_bool(value, cfg.security.enable_seccomp);
    if (key.find("seccomp_policy_path") != std::string::npos) cfg.security.seccomp_policy_path = value;
    if (key.find("cgroup_policy_path") != std::string::npos) cfg.security.cgroup_policy_path = value;
    if (key.find("bind_address") != std::string::npos) cfg.api.bind_address = value;
  }
  return cfg;
}

} // namespace ai_cloud::core
