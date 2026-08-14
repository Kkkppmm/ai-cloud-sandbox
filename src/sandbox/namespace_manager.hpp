#pragma once
#include <string>

namespace ai_cloud::sandbox {

class NamespaceManager {
 public:
  bool isolate_process(bool enable_network, std::string* error) const;
};

} // namespace ai_cloud::sandbox
