#ifndef GUIDING_BREEZE_SRC_CORE_GAME_H
#define GUIDING_BREEZE_SRC_CORE_GAME_H

#include "SDL_render.h"

class Game final {
public:
  ~Game();

  [[nodiscard]] bool IsRunning() const;

  void Start();
  void Update();
  void Exit();

private:
  bool was_started_{false};
  bool should_exit_{false};

  SDL_Window* window_{nullptr};
  SDL_Renderer* renderer_{nullptr};
};

#endif // GUIDING_BREEZE_SRC_CORE_GAME_H
