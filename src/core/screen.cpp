#include "screen.hpp"

#include "game.hpp"
#include "logger/logger.hpp"

#include "SDL_stdinc.h"
#include "SDL_video.h"

namespace gb::Screen {

namespace {

  Resolution resolution; //< Текущее разрешение экрана
  std::vector<Resolution> available_resolutions; //< Коллекция доступных разрешений экрана

} // namespace

const Resolution& GetResolution() {
  return resolution;
}

void SetResolution(const Resolution& requested_resolution, DisplayMode mode) {
  auto* window = Game::GetWindow();
  auto display_index = SDL_GetWindowDisplayIndex(window);

  SDL_DisplayMode requested_mode;
  requested_mode.format = 0;
  requested_mode.w = requested_resolution.width;
  requested_mode.h = requested_resolution.height;
  requested_mode.refresh_rate = requested_resolution.refresh_rate;
  requested_mode.driverdata = nullptr;

  SDL_DisplayMode final_mode;

  if (!SDL_GetClosestDisplayMode(display_index, &requested_mode, &final_mode)) {
    Logger::Error(
      "Не удалось найти ближайшее разрешение под [W:{} H:{} Hz:{} M:{}]...",
      requested_resolution.width, requested_resolution.height, requested_resolution.refresh_rate, static_cast<int>(mode)
    );
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

  Logger::Info(
    "Разрешение экрана изменено на [W:{} H:{} Hz:{} M:{}].",
    resolution.width, resolution.height, resolution.refresh_rate, static_cast<int>(mode)
  );
}

const std::vector<Resolution>& GetAvailableResolutions() {
  if (available_resolutions.empty()) {
    auto display_index = SDL_GetWindowDisplayIndex(Game::GetWindow());
    auto num_modes = SDL_GetNumDisplayModes(display_index);
    
    for (auto i = size_t{0}; i < num_modes; i++) {
      SDL_DisplayMode mode;

      if (!SDL_GetDisplayMode(display_index, i, &mode)) {
        auto& resolution = available_resolutions.emplace_back();
        resolution.width = mode.w;
        resolution.height = mode.h;
        resolution.refresh_rate = mode.refresh_rate;
      }
    }
  }
  
  return available_resolutions;
}

} // namespace gb::Screen
