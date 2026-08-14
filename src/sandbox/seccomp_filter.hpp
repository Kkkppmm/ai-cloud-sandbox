#pragma once
#include <string>

namespace ai_cloud::sandbox {

class SeccompFilter {
 public:
  bool apply_policy(const std::string& policy_path, std::string* error) const;
};

} // namespace ai_cloud::sandbox
