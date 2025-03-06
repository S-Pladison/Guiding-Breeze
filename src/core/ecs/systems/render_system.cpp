#include "render_system.hpp"

#include "core/game.hpp"

#include <cassert>

#include "SDL_image.h"

namespace gb {

RenderSystem::RenderSystem() {
  texture_ = IMG_LoadTexture(Game::GetRenderer(), "res/textures/test.png");
}

RenderSystem::~RenderSystem() {
  SDL_DestroyTexture(texture_);
}

void RenderSystem::Update(entt::registry& /*registry*/) {
  auto dest_rect = SDL_Rect{100, 100, 64, 64};
  SDL_RenderCopy(Game::GetRenderer(), texture_, nullptr, &dest_rect);
}

} // namespace gb