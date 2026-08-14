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
    if (key == "port") {
      try {
        cfg.api.port = std::stoi(value);
      } catch (...) {
      }
    }
    if (key == "memory_limit_mb") {
      try {
        cfg.resources.memory_limit_mb = static_cast<std::size_t>(std::stoull(value));
      } catch (...) {
      }
    }
    if (key == "cpu_shares") {
      try {
        cfg.resources.cpu_shares = std::stoi(value);
      } catch (...) {
      }
    }
    if (key == "workspace_quota_mb") {
      try {
        cfg.resources.workspace_quota_mb = static_cast<std::size_t>(std::stoull(value));
      } catch (...) {
      }
    }
    if (key == "enable_network") cfg.security.enable_network = parse_bool(value, cfg.security.enable_network);
    if (key == "enable_seccomp") cfg.security.enable_seccomp = parse_bool(value, cfg.security.enable_seccomp);
    if (key == "seccomp_policy_path") cfg.security.seccomp_policy_path = value;
    if (key == "cgroup_policy_path") cfg.security.cgroup_policy_path = value;
    if (key == "bind_address") cfg.api.bind_address = value;
  }
  return cfg;
}

} // namespace ai_cloud::core
