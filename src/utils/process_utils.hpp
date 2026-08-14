#pragma once
#include <string>
#include <vector>

namespace ai_cloud::utils {

struct CommandResult {
  int exit_code{-1};
  std::string output;
};

CommandResult run_command(const std::vector<std::string>& argv);

} // namespace ai_cloud::utils
