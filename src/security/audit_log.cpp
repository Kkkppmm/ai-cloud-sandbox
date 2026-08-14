#include "audit_log.hpp"
#include "utils/json_utils.hpp"
#include <chrono>
#include <fstream>

namespace ai_cloud::security {

AuditLog::AuditLog(std::string path) : path_(std::move(path)) {}

bool AuditLog::record(const std::string& user, const std::string& action, const std::string& result) {
  std::lock_guard<std::mutex> lock(mu_);
  std::ofstream out(path_, std::ios::app);
  if (!out.good()) return false;
  auto ts = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
  out << "{\"ts\":" << ts << ",\"user\":\"" << ai_cloud::utils::json_escape(user)
      << "\",\"action\":\"" << ai_cloud::utils::json_escape(action)
      << "\",\"result\":\"" << ai_cloud::utils::json_escape(result) << "\"}\n";
  return true;
}

} // namespace ai_cloud::security
