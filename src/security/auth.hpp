#pragma once
#include <string>
#include <unordered_set>

namespace ai_cloud::security {

class AuthManager {
 public:
  void add_api_key(std::string key);
  bool authenticate(const std::string& key) const;

 private:
  static bool constant_time_equal(const std::string& a, const std::string& b);
  std::unordered_set<std::string> keys_;
};

} // namespace ai_cloud::security
