#include "cgroup_manager.hpp"
#include <filesystem>
#include <fstream>

namespace ai_cloud::sandbox {

CgroupManager::CgroupManager(std::string root) : root_(std::move(root)) {}

bool CgroupManager::apply_limits(const std::string& agent_id, const core::ResourceConfig& cfg, std::string* error) const {
  namespace fs = std::filesystem;
  std::error_code ec;
  fs::create_directories(root_ + "/" + agent_id, ec);
  if (ec) {
    if (error) *error = ec.message();
    return false;
  }
  const std::string dir = root_ + "/" + agent_id;
  std::ofstream cpu(dir + "/cpu.weight");
  std::ofstream mem(dir + "/memory.max");
  if (!cpu.good() || !mem.good()) {
    if (error) *error = "failed to open cgroup control files";
    return false;
  }
  cpu << cfg.cpu_shares;
  mem << (cfg.memory_limit_mb * 1024 * 1024);
  return true;
}

} // namespace ai_cloud::sandbox
