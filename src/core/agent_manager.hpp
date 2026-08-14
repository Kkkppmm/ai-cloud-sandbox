#pragma once
#include "agent.hpp"
#include "tools/tool_executor.hpp"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace ai_cloud::core {

class AgentManager {
 public:
  AgentManager(std::size_t thread_count, tools::ToolExecutor executor);
  ~AgentManager();

  void start();
  void stop();
  void submit(std::shared_ptr<Agent> agent);

 private:
  void worker_loop();

  std::size_t thread_count_;
  tools::ToolExecutor executor_;
  std::mutex mu_;
  std::condition_variable cv_;
  std::queue<std::shared_ptr<Agent>> queue_;
  std::vector<std::thread> workers_;
  bool running_{false};
};

} // namespace ai_cloud::core
