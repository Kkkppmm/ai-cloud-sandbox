#pragma once
#include <string>

namespace ai_cloud::storage {

class FilesystemIsolation {
 public:
  bool validate_relative_path(const std::string& rel_path) const;
};

} // namespace ai_cloud::storage
