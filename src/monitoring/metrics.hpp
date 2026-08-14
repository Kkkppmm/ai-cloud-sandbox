#pragma once
#include <atomic>
#include <cstdint>

namespace ai_cloud::monitoring {

class Metrics {
 public:
  void inc_agents_started() { ++agents_started_; }
  void inc_agents_failed() { ++agents_failed_; }
  std::uint64_t agents_started() const { return agents_started_.load(); }
  std::uint64_t agents_failed() const { return agents_failed_.load(); }

 private:
  std::atomic<std::uint64_t> agents_started_{0};
  std::atomic<std::uint64_t> agents_failed_{0};
};

} // namespace ai_cloud::monitoring
