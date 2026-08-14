#pragma once
#include <string>

namespace ai_cloud::sandbox {

class SeccompFilter {
 public:
  bool apply_default_policy(std::string* error) const;
};

} // namespace ai_cloud::sandbox
