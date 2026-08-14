#include "routes.hpp"

namespace ai_cloud::api {

Routes::Routes() : allowed_{"/health", "/agents", "/agents/submit"} {}

bool Routes::is_allowed(const std::string& path) const { return allowed_.count(path) > 0; }

} // namespace ai_cloud::api
