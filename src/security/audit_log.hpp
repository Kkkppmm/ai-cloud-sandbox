#pragma once
#include <mutex>
#include <string>

namespace ai_cloud::security {

class AuditLog {
 public:
  explicit AuditLog(std::string path);
  bool record(const std::string& user, const std::string& action, const std::string& result);

 private:
  std::string path_;
  std::mutex mu_;
};

} // namespace ai_cloud::security
