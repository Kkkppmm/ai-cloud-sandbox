#pragma once
#include <stdexcept>
#include <string>

namespace ai_cloud::utils {

class Error : public std::runtime_error {
 public:
  explicit Error(const std::string& msg) : std::runtime_error(msg) {}
};

} // namespace ai_cloud::utils
