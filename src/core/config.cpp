#include "config.hpp"
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

namespace {
std::string trim(std::string s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
  return s;
}
} // namespace

namespace ai_cloud::core {

DaemonConfig load_config(const std::string& path) {
  DaemonConfig cfg;
  std::ifstream in(path);
  if (!in.good()) return cfg;

  std::string line;
  while (std::getline(in, line)) {
    auto pos = line.find(':');
    if (pos == std::string::npos) continue;
    std::string key = trim(line.substr(0, pos));
    std::string value = trim(line.substr(pos + 1));
    if (key.find("port") != std::string::npos) {
      try {
        cfg.api.port = std::stoi(value);
      } catch (...) {
      }
    }
    if (key.find("enable_network") != std::string::npos) cfg.security.enable_network = value.find("true") != std::string::npos;
  }
  return cfg;
}

} // namespace ai_cloud::core
