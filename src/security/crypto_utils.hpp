#pragma once
#include <string>

namespace ai_cloud::security {

class CryptoUtils {
 public:
  static std::string mask_secret(const std::string& input);
};

} // namespace ai_cloud::security
