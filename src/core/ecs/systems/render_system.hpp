#ifndef GUIDING_BREEZE_SRC_CORE_ECS_SYSTEMS_RENDER_SYSTEM_H
#define GUIDING_BREEZE_SRC_CORE_ECS_SYSTEMS_RENDER_SYSTEM_H

#include "SDL_render.h"

#include "core/ecs/systems/system.hpp"

namespace gb {

class RenderSystem final : public System {
private:
  SDL_Texture* texture_;

public:
  RenderSystem();
  ~RenderSystem() override;

public:
  void Update(entt::registry& registry) override;
};

} // namespace gb

#endif // GUIDING_BREEZE_SRC_CORE_ECS_SYSTEMS_RENDER_SYSTEM_H