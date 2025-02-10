#ifndef GUIDING_BREEZE_SRC_CORE_GAME_H
#define GUIDING_BREEZE_SRC_CORE_GAME_H

#include <cstddef>
#include "SDL_render.h"
#include <glm/ext/vector_int2.hpp>
#include <glm/vec2.hpp>

namespace gb {

class Game final {
public:
  Game(Game const&) = delete;
  Game& operator=(Game const&) = delete;

private:
  Game() = default;
  ~Game();

private:
  bool was_started_{false};
  bool should_exit_{false};

  SDL_Window* window_{nullptr};
  SDL_Renderer* renderer_{nullptr};

  glm::ivec2 screen_size_;

public:
  [[nodiscard]] static SDL_Window* GetWindow();

  [[nodiscard]] static bool IsRunning();

  static void Start();
  static void Update();
  static void Render();
  static void Exit();

private:
  static Game& GetInstance();
};

} // namespace gb

#endif // GUIDING_BREEZE_SRC_CORE_GAME_H
