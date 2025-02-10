#include "screen.hpp"
#include <vector>
#include "SDL_stdinc.h"
#include "SDL_video.h"
#include "core/game.hpp"
#include "logger/logger.hpp"

namespace gb::Screen {

namespace {

Resolution resolution;
std::vector<Resolution> available_resolutions;

}

const Resolution& GetResolution() {
  return resolution;
}

void SetResolution(size_t width, size_t height, DisplayMode mode) {
  auto* window = Game::GetWindow();
  auto display_index = SDL_GetWindowDisplayIndex(Game::GetWindow());

  SDL_DisplayMode requested_mode;
  requested_mode.format = 0;
  requested_mode.w = width;
  requested_mode.h = height;
  requested_mode.refresh_rate = 0;
  requested_mode.driverdata = nullptr;

  SDL_DisplayMode final_mode;

  if (!SDL_GetClosestDisplayMode(display_index, &requested_mode, &final_mode)) {
    Logger::Error("Не удалось найти ближайшее разрешение под [W:{} H:{} M:{}]...", width, height, static_cast<int>(mode));
    return;
  }

  switch (mode) {
    case DisplayMode::Fullscreen:
      {     
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        SDL_SetWindowBordered(window, SDL_TRUE);
        SDL_SetWindowDisplayMode(window, &final_mode);   
      }
      break;
    case DisplayMode::Borderless:
      {
        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowBordered(window, SDL_FALSE);
        SDL_SetWindowSize(window, final_mode.w, final_mode.h);
      }
      break;
    case DisplayMode::Windowed:
    default:
        {
          SDL_SetWindowFullscreen(window, 0);
          SDL_SetWindowBordered(window, SDL_TRUE);
          SDL_SetWindowSize(window, final_mode.w, final_mode.h);
        }
      break;
  }

  resolution.width = final_mode.w;
  resolution.height = final_mode.h;
  resolution.refresh_rate = final_mode.refresh_rate;

  Logger::Info("Разрешение экрана изменено на [W:{} H:{} M:{}].", width, height, static_cast<int>(mode));
}

const std::vector<Resolution>& GetAvailableResolutions() {
  if (available_resolutions.empty()) {
    int display_index = SDL_GetWindowDisplayIndex(Game::GetWindow());
    int num_modes = SDL_GetNumDisplayModes(display_index);
    
    for (int i = 0; i < num_modes; ++i) {
      SDL_DisplayMode mode;
      if (!SDL_GetDisplayMode(display_index, i, &mode)) {
        available_resolutions.push_back(
          {static_cast<size_t>(mode.w), static_cast<size_t>(mode.h), static_cast<size_t>(mode.refresh_rate)}
        );
      }
    }
  }
  return available_resolutions; 
}

} // namespace gb::Screen
