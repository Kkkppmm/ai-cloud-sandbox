#pragma once
#include <atomic>

namespace ai_cloud::api {

class GrpcServer {
 public:
  void start() { running_ = true; }
  void stop() { running_ = false; }
  bool running() const noexcept { return running_; }

 private:
  std::atomic<bool> running_{false};
};

} // namespace ai_cloud::api
