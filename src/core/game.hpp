#ifndef GUIDING_BREEZE_SRC_CORE_GAME_H
#define GUIDING_BREEZE_SRC_CORE_GAME_H

#include "SDL_render.h"

namespace gb::Game {

/**
 * @brief Получить указатель на окно игры.
 * 
 * @return SDL_Window* Указатель на окно игры.
 */
[[nodiscard]] SDL_Window* GetWindow();

/**
 * @brief Получить указатель на отрисовщик игры.
 * 
 * @return SDL_Renderer* Указатель на отрисовщик игры.
 */
[[nodiscard]] SDL_Renderer* GetRenderer();

/**
 * @brief Завершить работу игры.
 */
void Stop();

} // namespace gb::Game

#endif // GUIDING_BREEZE_SRC_CORE_GAME_H
