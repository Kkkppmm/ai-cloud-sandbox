#pragma once
#include <atomic>
#include <chrono>
#include <functional>

namespace ai_cloud::core {

class EventLoop {
 public:
  using TickFn = std::function<void()>;
  void run(TickFn tick, std::chrono::milliseconds interval);
  void stop();

 private:
  std::atomic<bool> running_{false};
};

} // namespace ai_cloud::core
