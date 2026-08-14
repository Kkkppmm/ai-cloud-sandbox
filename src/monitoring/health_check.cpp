#include "health_check.hpp"

namespace ai_cloud::monitoring {

void HealthCheck::add_probe(Probe probe) { probes_.push_back(std::move(probe)); }

bool HealthCheck::healthy() const {
  for (const auto& p : probes_) {
    if (!p()) return false;
  }
  return true;
}

} // namespace ai_cloud::monitoring
