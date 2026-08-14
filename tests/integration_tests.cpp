#include "test_framework.hpp"
#include "core/config.hpp"
#include "security/auth.hpp"
#include "security/crypto_utils.hpp"
#include "utils/json_utils.hpp"
#include <fstream>

TEST(auth_and_masking) {
  ai_cloud::security::AuthManager auth;
  auth.add_api_key("secret-key");
  REQUIRE(auth.authenticate("secret-key"));
  REQUIRE(!auth.authenticate("wrong"));
  REQUIRE(ai_cloud::security::CryptoUtils::mask_secret("abcdef").size() == 6);
}

TEST(config_load_tolerates_bad_port) {
  const std::string path = "/tmp/ai-cloud-config-test.yaml";
  std::ofstream out(path);
  out << "port: abc\n";
  out << "enable_network: true\n";
  out << "enable_seccomp: false\n";
  out << "bind_address: 0.0.0.0\n";
  out << "seccomp_policy_path: /tmp/seccomp.policy\n";
  out << "cgroup_policy_path: /tmp/cgroup.policy\n";
  out.close();
  auto cfg = ai_cloud::core::load_config(path);
  REQUIRE(cfg.api.port == 8080);
  REQUIRE(cfg.security.enable_network);
  REQUIRE(!cfg.security.enable_seccomp);
  REQUIRE(cfg.api.bind_address == "0.0.0.0");
  REQUIRE(cfg.security.seccomp_policy_path == "/tmp/seccomp.policy");
  REQUIRE(cfg.security.cgroup_policy_path == "/tmp/cgroup.policy");
}

TEST(json_escape_escapes_quotes) {
  auto s = ai_cloud::utils::to_json_object({{"k\"1", "v\\2"}});
  REQUIRE(s.find("\\\"") != std::string::npos);
  REQUIRE(s.find("\\\\") != std::string::npos);
}

int main() { return testfw::run_all(); }
