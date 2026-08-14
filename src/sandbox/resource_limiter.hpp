#pragma once
#include <cstddef>

namespace ai_cloud::sandbox {

class ResourceLimiter {
 public:
  bool set_memory_limit_mb(std::size_t limit_mb) const;
};

} // namespace ai_cloud::sandbox
