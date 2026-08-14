#include "crypto_utils.hpp"

namespace ai_cloud::security {

std::string CryptoUtils::mask_secret(const std::string& input) {
  if (input.size() <= 4) return "****";
  return std::string(input.size() - 4, '*') + input.substr(input.size() - 4);
}

} // namespace ai_cloud::security
