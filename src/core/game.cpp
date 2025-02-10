#include "game.hpp"

#include "core/screen.hpp"
#include "logger/logger.hpp"

#include <algorithm>
#include <string>

#include "SDL_video.h"
#include "fmt/format.h"
#include "imgui.h"

namespace gb {

namespace {

  SDL_Window* window{nullptr}; //< Указатель на окно игры
  SDL_Renderer* renderer{nullptr}; //< Указатель на отрисовщик игры

  bool should_exit; //< Флаг, указывающий на то, нужно ли прекратить игру после завершения текущего цикла

} // namespace

bool IsExitRequested() {
  return should_exit;
}

void RequestExit() {
  should_exit = true;
}

void OnStart(SDL_Window* window, SDL_Renderer* renderer) {
  gb::window = window;
  gb::renderer = renderer;

  Logger::Info("Запуск игры...");
  Screen::SetResolution(1280, 720, Screen::DisplayMode::Windowed);
}

void Update() {
  // ...
}

void Render() {
  ImGui::Begin("Настройки", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  
  ImGui::SeparatorText("Графика");

  // Получение данных о разрешениях
  const auto& resolutions = Screen::GetAvailableResolutions();
  const auto& active_resolution = Screen::GetResolution();
  static int selected_resolution_index = std::find_if(resolutions.begin(), resolutions.end(), [active_resolution](const auto& r) {
    return r.width == active_resolution.width && r.height == active_resolution.height && r.refresh_rate == active_resolution.refresh_rate;
  }) - resolutions.begin();

  // Отрисовка всплывающего списка для выбора разрешения
  if (ImGui::BeginCombo("Разрешение", fmt::format("{}x{} {}hz", resolutions[selected_resolution_index].width, resolutions[selected_resolution_index].height, resolutions[selected_resolution_index].refresh_rate).c_str())) {
    for (size_t i = 0; i < resolutions.size(); i++) {
      const auto& resolution = resolutions[i];
      auto selected = (selected_resolution_index == i);

      if (ImGui::Selectable(fmt::format("{}x{} {}hz", resolution.width, resolution.height, resolution.refresh_rate).c_str(), &selected)) {
        selected_resolution_index = i;
      }
    }
    ImGui::EndCombo();
  }

  // Отрисовка всплывающего списка выбора режима экрана
  static auto mode = Screen::DisplayMode::Windowed;
  static const char* modes[] = {"Windowed", "Borderless", "Fullscreen"};
  int item_current = static_cast<int>(mode);
  if (ImGui::Combo("Режим", &item_current, modes, IM_ARRAYSIZE(modes))) {
    mode = static_cast<Screen::DisplayMode>(item_current);
  }

  ImGui::Separator();

  // Кнопчка
  if (ImGui::Button("Применить")) {
    Screen::SetResolution(resolutions[selected_resolution_index], mode);
  }

  ImGui::End();

  // Отрисовка квадрата
  SDL_Rect rect{50, 50, 100, 100};
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
  SDL_RenderFillRect( renderer, &rect );
}

void OnExit() {
  gb::window = nullptr;
  gb::renderer = nullptr;

  Logger::Info("Завершение игры...");
}

namespace Game {

  SDL_Window* GetWindow() {
    return window;
  }

  SDL_Renderer* GetRenderer() {
    return renderer;
  }

  void Stop() {
    RequestExit();
  }

} // namespace Game

} // namespace gb