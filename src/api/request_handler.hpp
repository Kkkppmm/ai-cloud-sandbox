#pragma once
#include <map>
#include <string>

namespace ai_cloud::api {

struct Request {
  std::string path;
  std::string method;
  std::map<std::string, std::string> headers;
  std::string body;
};

struct Response {
  int status{200};
  std::string body;
};

class RequestHandler {
 public:
  Response handle(const Request& req) const;
};

} // namespace ai_cloud::api
