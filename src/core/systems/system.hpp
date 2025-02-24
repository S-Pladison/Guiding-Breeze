#ifndef GUIDING_BREEZE_SRC_CORE_SYSTEMS_SYSTEM_H
#define GUIDING_BREEZE_SRC_CORE_SYSTEMS_SYSTEM_H

#include "entt/entt.hpp"

namespace gb {

class System {
private:
  entt::registry* registry_;

public:
  explicit System(entt::registry* registry);
  System(const System&) = default;
  System(System&&) = default;
  virtual ~System() noexcept = default;

public:
  System& operator=(System&&) = delete;
  System& operator=(const System&) = delete;

public:
  virtual void Update() = 0;

protected:
  [[nodiscard]] entt::registry& GetRegistry() const;
};

} // namespace gb

#endif // GUIDING_BREEZE_SRC_CORE_SYSTEMS_SYSTEM_H