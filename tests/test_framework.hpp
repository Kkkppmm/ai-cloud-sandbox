#pragma once
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace testfw {

using TestFn = std::function<void()>;

inline std::vector<std::pair<std::string, TestFn>>& registry() {
  static std::vector<std::pair<std::string, TestFn>> r;
  return r;
}

struct Register {
  Register(std::string name, TestFn fn) { registry().emplace_back(std::move(name), std::move(fn)); }
};

inline int run_all() {
  int failed = 0;
  for (const auto& [name, fn] : registry()) {
    try {
      fn();
      std::cout << "[PASS] " << name << "\n";
    } catch (const std::exception& e) {
      ++failed;
      std::cout << "[FAIL] " << name << ": " << e.what() << "\n";
    }
  }
  return failed;
}

} // namespace testfw

#define TEST(name) \
  static void name(); \
  static testfw::Register reg_##name(#name, name); \
  static void name()

#define REQUIRE(cond)                                                                    \
  do {                                                                                   \
    if (!(cond)) throw std::runtime_error(std::string("require failed: ") + #cond);    \
  } while (0)
