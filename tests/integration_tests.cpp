#include "test_framework.hpp"
#include "security/auth.hpp"
#include "security/crypto_utils.hpp"

TEST(auth_and_masking) {
  ai_cloud::security::AuthManager auth;
  auth.add_api_key("secret-key");
  REQUIRE(auth.authenticate("secret-key"));
  REQUIRE(!auth.authenticate("wrong"));
  REQUIRE(ai_cloud::security::CryptoUtils::mask_secret("abcdef").size() == 6);
}

int main() { return testfw::run_all(); }
