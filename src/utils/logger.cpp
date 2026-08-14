#include "logger.hpp"
#include <iostream>

namespace ai_cloud::utils {

Logger& Logger::instance() {
  static Logger logger;
  return logger;
}

void Logger::info(const std::string& msg) {
  std::lock_guard<std::mutex> lock(mu_);
  std::cerr << "[INFO] " << msg << std::endl;
}

void Logger::error(const std::string& msg) {
  std::lock_guard<std::mutex> lock(mu_);
  std::cerr << "[ERROR] " << msg << std::endl;
}

} // namespace ai_cloud::utils
