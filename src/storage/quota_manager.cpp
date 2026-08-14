#include "quota_manager.hpp"

namespace ai_cloud::storage {

bool QuotaManager::can_write(const Workspace& ws, std::size_t bytes) const { return ws.within_quota(bytes); }

} // namespace ai_cloud::storage
