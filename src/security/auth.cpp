#include "auth.hpp"

namespace ai_cloud::security {

void AuthManager::add_api_key(std::string key) { keys_.insert(std::move(key)); }

bool AuthManager::constant_time_equal(const std::string& a, const std::string& b) {
  if (a.size() != b.size()) return false;
  unsigned char diff = 0;
  for (std::size_t i = 0; i < a.size(); ++i) diff |= static_cast<unsigned char>(a[i] ^ b[i]);
  return diff == 0;
}

bool AuthManager::authenticate(const std::string& key) const {
  for (const auto& known : keys_) {
    if (constant_time_equal(known, key)) return true;
  }
  return false;
}

} // namespace ai_cloud::security
