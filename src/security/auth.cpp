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
  std::size_t size_diff = a.size() ^ b.size();
  while (size_diff != 0) {
    diff |= static_cast<unsigned char>(size_diff & 0xFFU);
    size_diff >>= 8U;
  }
  return diff == 0;
}

bool AuthManager::authenticate(const std::string& key) const {
  for (const auto& known : keys_) {
    if (constant_time_equal(known, key)) return true;
  }
  return false;
}

} // namespace ai_cloud::security
