#pragma once
#include <map>
#include <string>

namespace ai_cloud::utils {

std::string to_json_object(const std::map<std::string, std::string>& values);

} // namespace ai_cloud::utils
