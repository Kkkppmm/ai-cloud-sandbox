#include "filesystem_isolation.hpp"
#include <filesystem>

namespace ai_cloud::storage {

bool FilesystemIsolation::validate_relative_path(const std::string& rel_path) const {
  if (rel_path.empty()) return false;
  std::filesystem::path p(rel_path);
  if (p.is_absolute()) return false;
  for (const auto& part : p) {
    if (part == "..") return false;
  }
  return true;
}

} // namespace ai_cloud::storage
