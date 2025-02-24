#include "system.hpp"
#include <cassert>

namespace gb {

System::System(entt::registry* registry) : registry_(registry) {
  assert(registry);
}

entt::registry& System::GetRegistry() const {
  return *registry_;
}

} // namespace gb