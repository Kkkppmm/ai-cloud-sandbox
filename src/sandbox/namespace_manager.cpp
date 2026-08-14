#include "namespace_manager.hpp"
#include <sched.h>
#include <cerrno>
#include <cstring>

namespace ai_cloud::sandbox {

bool NamespaceManager::isolate_process(bool enable_network, std::string* error) const {
  int flags = CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWNS | CLONE_NEWPID;
  if (!enable_network) flags |= CLONE_NEWNET;
  if (unshare(flags) != 0) {
    if (error) *error = std::string("unshare failed: ") + std::strerror(errno);
    return false;
  }
  return true;
}

} // namespace ai_cloud::sandbox
