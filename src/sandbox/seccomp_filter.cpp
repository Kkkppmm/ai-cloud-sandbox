#include "seccomp_filter.hpp"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <cstddef>
#include <cctype>
#include <fstream>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <string>
#include <sys/prctl.h>
#include <sys/syscall.h>

namespace ai_cloud::sandbox {
namespace {

std::string trim(std::string s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
  return s;
}

std::string to_lower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
  return s;
}

bool load_mode(const std::string& policy_path, std::string* mode, std::string* error) {
  if (policy_path.empty()) return true;
  std::ifstream in(policy_path);
  if (!in.good()) {
    if (error) *error = "failed to open seccomp policy file: " + policy_path;
    return false;
  }
  std::string line;
  while (std::getline(in, line)) {
    line = trim(line);
    if (line.empty() || line[0] == '#') continue;
    auto pos = line.find('=');
    if (pos == std::string::npos) pos = line.find(':');
    if (pos == std::string::npos) continue;
    const auto key = to_lower(trim(line.substr(0, pos)));
    const auto value = to_lower(trim(line.substr(pos + 1)));
    if (key == "mode") {
      *mode = value;
      return true;
    }
  }
  return true;
}

bool apply_strict(std::string* error) {
  if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_STRICT) != 0) {
    if (error) *error = std::string("PR_SET_SECCOMP strict failed: ") + std::strerror(errno);
    return false;
  }
  return true;
}

bool apply_filter(std::string* error) {
#if !defined(__x86_64__)
  if (error) *error = "seccomp filter mode is currently supported only on x86_64";
  return false;
#else
  static sock_filter filter[] = {
      BPF_STMT(BPF_LD | BPF_W | BPF_ABS, static_cast<unsigned int>(offsetof(seccomp_data, arch))),
      BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, AUDIT_ARCH_X86_64, 0, 7),
      BPF_STMT(BPF_LD | BPF_W | BPF_ABS, static_cast<unsigned int>(offsetof(seccomp_data, nr))),
      BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_read, 4, 0),
      BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_write, 3, 0),
      BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_exit, 2, 0),
      BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_exit_group, 1, 0),
      BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_rt_sigreturn, 0, 1),
      BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),
      BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL_PROCESS),
  };

  sock_fprog prog{};
  prog.len = static_cast<unsigned short>(sizeof(filter) / sizeof(filter[0]));
  prog.filter = filter;

  if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog) != 0) {
    if (error) *error = std::string("PR_SET_SECCOMP filter failed: ") + std::strerror(errno);
    return false;
  }
  return true;
#endif
}

} // namespace

bool SeccompFilter::apply_policy(const std::string& policy_path, std::string* error) const {
  std::string mode = "strict";
  if (!load_mode(policy_path, &mode, error)) return false;
  if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) != 0) {
    if (error) *error = std::string("PR_SET_NO_NEW_PRIVS failed: ") + std::strerror(errno);
    return false;
  }
  if (mode == "strict") return apply_strict(error);
  if (mode == "filter") return apply_filter(error);
  if (error) *error = "unsupported seccomp mode in policy: " + mode;
  return false;
}

} // namespace ai_cloud::sandbox
