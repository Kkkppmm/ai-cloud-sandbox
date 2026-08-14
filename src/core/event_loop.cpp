#include "event_loop.hpp"
#include <thread>

namespace ai_cloud::core {

void EventLoop::run(TickFn tick, std::chrono::milliseconds interval) {
  running_ = true;
  while (running_) {
    tick();
    std::this_thread::sleep_for(interval);
  }
}

void EventLoop::stop() { running_ = false; }

} // namespace ai_cloud::core
