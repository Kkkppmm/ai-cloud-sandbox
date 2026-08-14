#include "config.hpp"
#include <fstream>
#include <string>

namespace ai_cloud::core {

DaemonConfig load_config(const std::string& path) {
  DaemonConfig cfg;
  std::ifstream in(path);
  if (!in.good()) return cfg;

  std::string line;
  while (std::getline(in, line)) {
    auto pos = line.find(':');
    if (pos == std::string::npos) continue;
    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);
    if (key.find("port") != std::string::npos) cfg.api.port = std::stoi(value);
    if (key.find("enable_network") != std::string::npos) cfg.security.enable_network = value.find("true") != std::string::npos;
  }
  return cfg;
}

} // namespace ai_cloud::core
