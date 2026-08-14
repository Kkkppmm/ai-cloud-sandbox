#include "agent.hpp"
#include <future>
#include <thread>

namespace ai_cloud::core {

Agent::Agent(std::string id, WorkFn work, std::chrono::milliseconds timeout)
    : id_(std::move(id)), work_(std::move(work)), timeout_(timeout) {}

bool Agent::run() {
  state_ = AgentState::Running;
  auto token = std::make_shared<std::atomic<bool>>(false);
  {
    std::lock_guard<std::mutex> lock(token_mu_);
    stop_token_ = token;
  }
  std::packaged_task<bool()> task([work = work_, token] { return work(token); });
  auto fut = task.get_future();
  std::thread worker(std::move(task));
  if (fut.wait_for(timeout_) == std::future_status::timeout) {
    token->store(true);
    state_ = AgentState::Timeout;
    worker.detach();
    return false;
  }
  worker.join();
  bool ok = fut.get();
  {
    std::lock_guard<std::mutex> lock(token_mu_);
    stop_token_.reset();
  }
  state_ = ok ? AgentState::Completed : AgentState::Failed;
  return ok;
}

void Agent::stop() {
  std::lock_guard<std::mutex> lock(token_mu_);
  if (stop_token_) stop_token_->store(true);
  state_ = AgentState::Stopped;
}

} // namespace ai_cloud::core
