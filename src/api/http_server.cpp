#include "http_server.hpp"

namespace ai_cloud::api {

HttpServer::HttpServer(RequestHandler handler) : handler_(std::move(handler)) {}

void HttpServer::start() { running_ = true; }
void HttpServer::stop() { running_ = false; }

} // namespace ai_cloud::api
