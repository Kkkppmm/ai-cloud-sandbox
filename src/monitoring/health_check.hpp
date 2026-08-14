#pragma once
#include <functional>
#include <string>
#include <vector>

namespace ai_cloud::monitoring {

class HealthCheck {
 public:
  using Probe = std::function<bool()>;
  void add_probe(Probe probe);
  bool healthy() const;

 private:
  std::vector<Probe> probes_;
};

} // namespace ai_cloud::monitoring
