#include "filesystem_isolation.hpp"

namespace ai_cloud::storage {

bool FilesystemIsolation::validate_relative_path(const std::string& rel_path) const {
  return !rel_path.empty() && rel_path.find("..") == std::string::npos && rel_path.front() != '/';
}

} // namespace ai_cloud::storage
