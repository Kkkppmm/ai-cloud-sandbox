#include "agent_manager.hpp"

namespace ai_cloud::core {

AgentManager::AgentManager(std::size_t thread_count, tools::ToolExecutor executor)
    : thread_count_(thread_count), executor_(std::move(executor)) {}

AgentManager::~AgentManager() { stop(); }

void AgentManager::start() {
  std::lock_guard<std::mutex> lock(mu_);
  if (running_) return;
  running_ = true;
  for (std::size_t i = 0; i < thread_count_; ++i) {
    workers_.emplace_back(&AgentManager::worker_loop, this);
  }
}

void AgentManager::submit(std::shared_ptr<Agent> agent) {
  {
    std::lock_guard<std::mutex> lock(mu_);
    queue_.push(std::move(agent));
  }
  cv_.notify_one();
}

void AgentManager::stop() {
  {
    std::lock_guard<std::mutex> lock(mu_);
    if (!running_) return;
    running_ = false;
  }
  cv_.notify_all();
  for (auto& worker : workers_) {
    if (worker.joinable()) worker.join();
  }
  workers_.clear();
}

void AgentManager::worker_loop() {
  while (true) {
    std::shared_ptr<Agent> agent;
    {
      std::unique_lock<std::mutex> lock(mu_);
      cv_.wait(lock, [&] { return !running_ || !queue_.empty(); });
      if (!running_ && queue_.empty()) return;
      agent = queue_.front();
      queue_.pop();
    }
    if (agent) (void)agent->run();
  }
}

} // namespace ai_cloud::core
