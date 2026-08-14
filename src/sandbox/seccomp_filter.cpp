#include "seccomp_filter.hpp"
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include <cerrno>
#include <cstring>

namespace ai_cloud::sandbox {

bool SeccompFilter::apply_default_policy(std::string* error) const {
  if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) != 0) {
    if (error) *error = std::string("PR_SET_NO_NEW_PRIVS failed: ") + std::strerror(errno);
    return false;
  }
  if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_STRICT) != 0) {
    if (error) *error = std::string("PR_SET_SECCOMP strict failed: ") + std::strerror(errno);
    return false;
  }
  return true;
}

} // namespace ai_cloud::sandbox
