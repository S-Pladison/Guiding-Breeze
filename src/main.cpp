#include "core/game.hpp"

int main(int, char**) {
  using gb::Game;

  Game::Start();

  while (Game::IsRunning()) {
    Game::Update();
    Game::Render();
  }

  Game::Exit();

  return EXIT_SUCCESS;
}