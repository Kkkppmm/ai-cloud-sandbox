#include "cgroup_manager.hpp"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <map>

namespace ai_cloud::sandbox {
namespace {

std::string trim(std::string s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
  return s;
}

bool write_value(const std::string& path, const std::string& value) {
  std::ofstream out(path);
  if (!out.good()) return false;
  out << value;
  return out.good();
}

bool load_policy(const std::string& path, std::map<std::string, std::string>* policy, std::string* error) {
  if (path.empty()) return true;
  std::ifstream in(path);
  if (!in.good()) {
    if (error) *error = "failed to open cgroup policy file: " + path;
    return false;
  }
  std::string line;
  while (std::getline(in, line)) {
    line = trim(line);
    if (line.empty() || line[0] == '#') continue;
    const auto eq = line.find('=');
    if (eq == std::string::npos) continue;
    const std::string key = trim(line.substr(0, eq));
    const std::string value = trim(line.substr(eq + 1));
    if (!key.empty() && !value.empty()) (*policy)[key] = value;
  }
  return true;
}

} // namespace

CgroupManager::CgroupManager(std::string root) : root_(std::move(root)) {}

bool CgroupManager::apply_limits(const std::string& agent_id,
                                 const core::ResourceConfig& cfg,
                                 const std::string& policy_path,
                                 std::string* error) const {
  namespace fs = std::filesystem;
  std::error_code ec;
  fs::create_directories(root_ + "/" + agent_id, ec);
  if (ec) {
    if (error) *error = ec.message();
    return false;
  }
  const std::string dir = root_ + "/" + agent_id;

  std::map<std::string, std::string> policy{
      {"cpu_weight_file", "cpu.weight"},
      {"memory_max_file", "memory.max"},
  };
  if (!load_policy(policy_path, &policy, error)) return false;

  const auto cpu_file = policy.find("cpu_weight_file");
  const auto mem_file = policy.find("memory_max_file");
  if (cpu_file == policy.end() || mem_file == policy.end()) {
    if (error) *error = "cgroup policy missing required keys";
    return false;
  }

  if (!write_value(dir + "/" + cpu_file->second, std::to_string(cfg.cpu_shares))) {
    if (error) *error = "failed to write cpu cgroup control";
    return false;
  }
  if (!write_value(dir + "/" + mem_file->second, std::to_string(cfg.memory_limit_mb * 1024ULL * 1024ULL))) {
    if (error) *error = "failed to write memory cgroup control";
    return false;
  }
  return true;
}

} // namespace ai_cloud::sandbox
