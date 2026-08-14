#include "string_utils.hpp"

namespace ai_cloud::utils {

std::string trim(std::string s) {
  const auto left = s.find_first_not_of(" \t\n\r");
  if (left == std::string::npos) return "";
  const auto right = s.find_last_not_of(" \t\n\r");
  return s.substr(left, right - left + 1);
}

} // namespace ai_cloud::utils
