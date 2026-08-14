#include "json_utils.hpp"
#include <sstream>

namespace ai_cloud::utils {

std::string json_escape(const std::string& input) {
  std::ostringstream ss;
  for (char c : input) {
    switch (c) {
      case '"':
        ss << "\\\"";
        break;
      case '\\':
        ss << "\\\\";
        break;
      case '\n':
        ss << "\\n";
        break;
      case '\r':
        ss << "\\r";
        break;
      case '\t':
        ss << "\\t";
        break;
      default:
        ss << c;
        break;
    }
  }
  return ss.str();
}

std::string to_json_object(const std::map<std::string, std::string>& values) {
  std::ostringstream ss;
  ss << "{";
  bool first = true;
  for (const auto& [k, v] : values) {
    if (!first) ss << ",";
    first = false;
    ss << "\"" << json_escape(k) << "\":\"" << json_escape(v) << "\"";
  }
  ss << "}";
  return ss.str();
}

} // namespace ai_cloud::utils
