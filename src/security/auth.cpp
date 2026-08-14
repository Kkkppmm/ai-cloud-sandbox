#include "auth.hpp"

namespace ai_cloud::security {

void AuthManager::add_api_key(std::string key) { keys_.insert(std::move(key)); }

bool AuthManager::constant_time_equal(const std::string& a, const std::string& b) {
  unsigned char diff = 0;
  const std::size_t max_len = a.size() > b.size() ? a.size() : b.size();
  for (std::size_t i = 0; i < max_len; ++i) {
    const unsigned char ac = i < a.size() ? static_cast<unsigned char>(a[i]) : 0U;
    const unsigned char bc = i < b.size() ? static_cast<unsigned char>(b[i]) : 0U;
    diff |= static_cast<unsigned char>(ac ^ bc);
  }
  diff |= static_cast<unsigned char>(a.size() ^ b.size());
  return diff == 0;
}

bool AuthManager::authenticate(const std::string& key) const {
  for (const auto& known : keys_) {
    if (constant_time_equal(known, key)) return true;
  }
  return false;
}

} // namespace ai_cloud::security
