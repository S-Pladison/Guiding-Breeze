#ifndef GUIDING_BREEZE_SRC_CORE_SYSTEMS_TEST_SYSTEM_H
#define GUIDING_BREEZE_SRC_CORE_SYSTEMS_TEST_SYSTEM_H

#include "logger/logger.hpp"

#include "core/components/test_component.hpp"
#include "core/systems/system.hpp"

namespace gb {

class TestSystem final : public System {
public:
  explicit TestSystem(entt::registry* registry) : System(registry) {}
  ~TestSystem() override = default;

public:
  void Update() override {
    GetRegistry().view<TestComponent>().each([](TestComponent& test) {
      test.value += 1;

      if (test.value % 100 == 0) {
        Logger::Info("{}", test.value);
      }
    });
  }
};

} // namespace gb

#endif // GUIDING_BREEZE_SRC_CORE_SYSTEMS_TEST_SYSTEM_H