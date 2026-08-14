#pragma once
#include <string>

namespace ai_cloud::utils {

struct CommandResult {
  int exit_code{-1};
  std::string output;
};

CommandResult run_command(const std::string& command);

} // namespace ai_cloud::utils
