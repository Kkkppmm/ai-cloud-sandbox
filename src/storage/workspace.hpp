#pragma once
#include <cstddef>
#include <string>

namespace ai_cloud::storage {

class Workspace {
 public:
  Workspace(std::string root, std::size_t quota_bytes);
  const std::string& root() const noexcept { return root_; }
  bool ensure_exists() const;
  bool atomic_write(const std::string& rel_path, const std::string& content, std::string* error);
  std::size_t usage_bytes() const;
  bool within_quota(std::size_t extra_bytes = 0) const;

 private:
  std::string root_;
  std::size_t quota_bytes_;
};

} // namespace ai_cloud::storage
