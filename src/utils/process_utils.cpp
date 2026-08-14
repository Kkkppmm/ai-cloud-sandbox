#include "process_utils.hpp"
#include <array>
#include <cstdio>

namespace ai_cloud::utils {

CommandResult run_command(const std::string& command) {
  CommandResult result;
  std::array<char, 256> buffer{};
  FILE* pipe = popen(command.c_str(), "r");
  if (!pipe) return result;
  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe)) {
    result.output += buffer.data();
  }
  result.exit_code = pclose(pipe);
  return result;
}

} // namespace ai_cloud::utils
