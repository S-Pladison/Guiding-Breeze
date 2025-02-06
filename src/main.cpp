#include "core/game.hpp"

int main(int, char**) {
  Game game;
  game.Start();

  while (game.IsRunning()) {
    game.Update();
  }

  game.Exit();

  return 0;
}