#pragma once
#include <mutex>
#include <string>

namespace ai_cloud::utils {

class Logger {
 public:
  static Logger& instance();
  void info(const std::string& msg);
  void error(const std::string& msg);

 private:
  std::mutex mu_;
};

} // namespace ai_cloud::utils
