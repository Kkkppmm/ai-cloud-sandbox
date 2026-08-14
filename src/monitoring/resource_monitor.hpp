#pragma once
#include <cstddef>

namespace ai_cloud::monitoring {

struct ResourceSnapshot {
  std::size_t rss_bytes{0};
  double cpu_percent{0.0};
};

class ResourceMonitor {
 public:
  ResourceSnapshot current() const;
};

} // namespace ai_cloud::monitoring
