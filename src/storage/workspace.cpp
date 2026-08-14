#include "workspace.hpp"
#include "filesystem_isolation.hpp"
#include <filesystem>
#include <fstream>

namespace ai_cloud::storage {

Workspace::Workspace(std::string root, std::size_t quota_bytes)
    : root_(std::move(root)), quota_bytes_(quota_bytes) {}

bool Workspace::ensure_exists() const {
  std::error_code ec;
  std::filesystem::create_directories(root_, ec);
  return !ec;
}

std::size_t Workspace::usage_bytes() const {
  std::size_t total = 0;
  std::error_code ec;
  if (!std::filesystem::exists(root_, ec)) return 0;
  for (const auto& entry : std::filesystem::recursive_directory_iterator(root_, ec)) {
    if (ec) break;
    if (entry.is_regular_file(ec)) total += static_cast<std::size_t>(entry.file_size(ec));
  }
  return total;
}

bool Workspace::within_quota(std::size_t extra_bytes) const {
  return usage_bytes() + extra_bytes <= quota_bytes_;
}

bool Workspace::atomic_write(const std::string& rel_path, const std::string& content, std::string* error) {
  FilesystemIsolation isolation;
  if (!isolation.validate_relative_path(rel_path)) {
    if (error) *error = "invalid relative path";
    return false;
  }
  if (!within_quota(content.size())) {
    if (error) *error = "quota exceeded";
    return false;
  }
  std::filesystem::path target = std::filesystem::path(root_) / rel_path;
  std::error_code ec;
  std::filesystem::create_directories(target.parent_path(), ec);
  if (ec) {
    if (error) *error = ec.message();
    return false;
  }
  auto tmp = target;
  tmp += ".tmp";
  {
    std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
    if (!out.good()) {
      if (error) *error = "cannot write temp file";
      return false;
    }
    out << content;
  }
  std::filesystem::rename(tmp, target, ec);
  if (ec) {
    if (error) *error = ec.message();
    return false;
  }
  return true;
}

} // namespace ai_cloud::storage
