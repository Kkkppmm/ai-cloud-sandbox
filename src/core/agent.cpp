#include "agent.hpp"
#include <future>
#include <thread>

namespace ai_cloud::core {

Agent::Agent(std::string id, WorkFn work, std::chrono::milliseconds timeout)
    : id_(std::move(id)), work_(std::move(work)), timeout_(timeout) {}

bool Agent::run() {
  if (stop_requested_) {
    state_ = AgentState::Stopped;
    return false;
  }
  state_ = AgentState::Running;
  std::packaged_task<bool()> task(work_);
  auto fut = task.get_future();
  std::thread worker(std::move(task));
  if (fut.wait_for(timeout_) == std::future_status::timeout) {
    state_ = AgentState::Timeout;
    worker.detach();
    return false;
  }
  worker.join();
  bool ok = fut.get();
  state_ = ok ? AgentState::Completed : AgentState::Failed;
  return ok;
}

void Agent::stop() {
  stop_requested_ = true;
  state_ = AgentState::Stopped;
}

} // namespace ai_cloud::core
