#pragma once
#include "workspace.hpp"

namespace ai_cloud::storage {

class QuotaManager {
 public:
  bool can_write(const Workspace& ws, std::size_t bytes) const;
};

} // namespace ai_cloud::storage
