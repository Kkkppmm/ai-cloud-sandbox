#include "json_utils.hpp"
#include <sstream>

namespace ai_cloud::utils {

std::string to_json_object(const std::map<std::string, std::string>& values) {
  std::ostringstream ss;
  ss << "{";
  bool first = true;
  for (const auto& [k, v] : values) {
    if (!first) ss << ",";
    first = false;
    ss << "\"" << k << "\":\"" << v << "\"";
  }
  ss << "}";
  return ss.str();
}

} // namespace ai_cloud::utils
