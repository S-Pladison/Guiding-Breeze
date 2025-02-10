#include "game.hpp"

#include "core/screen.hpp"
#include "logger/logger.hpp"

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
}

void Update() {
  // ...
}

void Render() {
  // Создание ImGui интерфейса
  ImGui::Begin("Hello, world!");
  
  // Отрисовываем Combo для вариантов доступных разрешений экрана
  const auto& resolution = Screen::GetAvailableResolutions();
  static int selected_index = 0;
  if (ImGui::BeginCombo("Resolution", fmt::format("{}x{} {}hz", resolution[selected_index].width, resolution[selected_index].height, resolution[selected_index].refresh_rate).c_str())) {
    for (int i = 0; i < resolution.size(); ++i) {
      const auto& res = resolution[i];
      bool selected = (selected_index == i);
      if (ImGui::Selectable(fmt::format("{}x{} {}hz", res.width, res.height, res.refresh_rate).c_str(), &selected)) {
        selected_index = i;
      }
    }
    ImGui::EndCombo();
  }

  // Создаем Combo для enum Screen::DisplayMode
  static Screen::DisplayMode mode = Screen::DisplayMode::Windowed;
  const char* items[] = {"Windowed", "Borderless", "Fullscreen"};
  int item_current = static_cast<int>(mode);
  ImGui::Combo("Display Mode", &item_current, items, IM_ARRAYSIZE(items));
  mode = static_cast<Screen::DisplayMode>(item_current);

  // Кнопчка
  if (ImGui::Button("Resize Window")) {
    Screen::SetResolution(resolution[selected_index], static_cast<Screen::DisplayMode>(mode));
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