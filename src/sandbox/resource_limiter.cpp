#include "resource_limiter.hpp"
#include <sys/resource.h>

namespace ai_cloud::sandbox {

bool ResourceLimiter::set_memory_limit_mb(std::size_t limit_mb) const {
  rlimit r{};
  r.rlim_cur = r.rlim_max = static_cast<rlim_t>(limit_mb) * 1024 * 1024;
  return setrlimit(RLIMIT_AS, &r) == 0;
}

} // namespace ai_cloud::sandbox
