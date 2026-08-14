#pragma once
#include "request_handler.hpp"
#include <atomic>

namespace ai_cloud::api {

class HttpServer {
 public:
  explicit HttpServer(RequestHandler handler);
  void start();
  void stop();
  bool running() const noexcept { return running_; }

 private:
  RequestHandler handler_;
  std::atomic<bool> running_{false};
};

} // namespace ai_cloud::api
