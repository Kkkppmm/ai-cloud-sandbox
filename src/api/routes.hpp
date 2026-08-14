#pragma once
#include <set>
#include <string>

namespace ai_cloud::api {

class Routes {
 public:
  Routes();
  bool is_allowed(const std::string& path) const;

 private:
  std::set<std::string> allowed_;
};

} // namespace ai_cloud::api
