#ifndef GUIDING_BREEZE_SRC_CORE_ECS_SYSTEMS_SYSTEM_H
#define GUIDING_BREEZE_SRC_CORE_ECS_SYSTEMS_SYSTEM_H

#include "entt/entt.hpp"

namespace gb {

class System {
public:
  System() = default;
  virtual ~System() noexcept = default;

public:
  System& operator=(System&&) = delete;
  System& operator=(const System&) = delete;

public:
  virtual void Update(entt::registry& registry) = 0;
};

} // namespace gb

#endif // GUIDING_BREEZE_SRC_CORE_ECS_SYSTEMS_SYSTEM_H