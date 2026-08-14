#include "process_utils.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace ai_cloud::utils {

CommandResult run_command(const std::vector<std::string>& argv) {
  CommandResult result;
  if (argv.empty()) return result;

  int pipefd[2];
  if (pipe(pipefd) != 0) return result;

  pid_t pid = fork();
  if (pid == -1) {
    close(pipefd[0]);
    close(pipefd[1]);
    return result;
  }

  if (pid == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    dup2(pipefd[1], STDERR_FILENO);
    close(pipefd[1]);

    std::vector<char*> args;
    args.reserve(argv.size() + 1);
    for (const auto& part : argv) args.push_back(const_cast<char*>(part.c_str()));
    args.push_back(nullptr);
    execvp(args[0], args.data());
    _exit(127);
  }

  close(pipefd[1]);
  char buffer[256];
  ssize_t n = 0;
  while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
    result.output.append(buffer, static_cast<std::size_t>(n));
  }
  close(pipefd[0]);

  int status = 0;
  if (waitpid(pid, &status, 0) != -1) {
    if (WIFEXITED(status)) {
      result.exit_code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
      result.exit_code = 128 + WTERMSIG(status);
    }
  }
  return result;
}

} // namespace ai_cloud::utils
