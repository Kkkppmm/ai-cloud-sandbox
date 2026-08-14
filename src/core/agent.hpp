#pragma once
#include <atomic>
#include <chrono>
#include <functional>
#include <string>

namespace ai_cloud::core {

enum class AgentState { Created, Running, Completed, Failed, Timeout, Stopped };

class Agent {
 public:
  using WorkFn = std::function<bool()>;

  Agent(std::string id, WorkFn work, std::chrono::milliseconds timeout);
  bool run();
  void stop();

  const std::string& id() const noexcept { return id_; }
  AgentState state() const noexcept { return state_.load(); }

 private:
  std::string id_;
  WorkFn work_;
  std::chrono::milliseconds timeout_;
  std::atomic<AgentState> state_{AgentState::Created};
  std::atomic<bool> stop_requested_{false};
};

} // namespace ai_cloud::core
