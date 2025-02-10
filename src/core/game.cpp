#include "game.hpp"

#include "SDL_video.h"
#include "core/screen.hpp"
#include "fmt/format.h"
#include "logger/logger.hpp"

#include "SDL.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

namespace gb {

Game::~Game() {
  Exit();
}

SDL_Window* Game::GetWindow() {
  return Game::GetInstance().window_;
}

bool Game::IsRunning() {
  auto& game = Game::GetInstance();
  return game.was_started_;
}

void Game::Start() {
  auto& game = Game::GetInstance();

  if (game.was_started_) {
    return;
  }

  Logger::Info("Подготовка перед запуском игры...");

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    Logger::Fatal("Не удалось инициализировать SDL...");
    return;
  }

  if (game.window_ = SDL_CreateWindow("Guiding Breeze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE); !game.window_) {
    Logger::Fatal("Не удалось инициализировать окно...");
    return;
  }

  SDL_GetWindowSize(game.window_, &game.screen_size_.x, &game.screen_size_.y);

  if (game.renderer_ = SDL_CreateRenderer(game.window_, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED); !game.renderer_) {
    Logger::Fatal("Не удалось инициализировать средство визуализации...");
    return;
  }

  // Инициализация ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();

  // Временно убираем сохранения данных ImGui
  io.IniFilename = nullptr;

  // Исправление кириллицы
  io.Fonts->AddFontFromFileTTF("res/fonts/minecraft_seven.ttf", 16.0F, nullptr, io.Fonts->GetGlyphRangesCyrillic());

  // Инициализация ImGui для SDL2 и SDL_Renderer
  ImGui_ImplSDL2_InitForSDLRenderer(game.window_, game.renderer_);
  ImGui_ImplSDLRenderer2_Init(game.renderer_);

  Logger::Info("Подготовка перед запуском игры завершена!");

  game.was_started_ = true;
}

void Game::Update() {
  auto& game = Game::GetInstance();

  if (!game.was_started_) {
    return;
  }

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT) game.should_exit_ = true;
  }
}

void Game::Render() {
  auto& game = Game::GetInstance();

  if (!game.was_started_) {
    return;
  }

  // Начало нового кадра ImGui
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  // Создание ImGui интерфейса
  ImGui::Begin("Hello, world!");
  
  const auto& resolution = Screen::GetAvailableResolutions();
  static int selected_index = -1;

  if (ImGui::BeginCombo("Resolution", resolution.empty() ? "None" : fmt::format("{}x{} {}hz", resolution[selected_index].width, resolution[selected_index].height, resolution[selected_index].refresh_rate).c_str())) {
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
  

  if (ImGui::Button("Resize Window")) {
    Screen::SetResolution(resolution[selected_index].width, resolution[selected_index].height, static_cast<Screen::DisplayMode>(mode));
  }
  ImGui::End();

  // Чистка экрана
  SDL_SetRenderDrawColor(game.renderer_, 0, 0, 0, 255);
  SDL_RenderClear(game.renderer_);

  // Отрисовка квадрата
  SDL_Rect rect{50, 50, 100, 100};
  SDL_SetRenderDrawColor( game.renderer_, 255, 0, 0, 0);
  SDL_RenderFillRect( game.renderer_, &rect );

  // Отрисовка ImGui
  ImGui::Render();
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), game.renderer_);

  // Update the window now with the content of the display
  SDL_RenderPresent(game.renderer_);

  if (game.should_exit_) {
    Exit();
  }
}

void Game::Exit() {
  auto& game = Game::GetInstance();

  if (!game.was_started_) {
    return;
  }

  Logger::Info("Завершение работы игры...");

  // Очистка
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(game.renderer_);
  SDL_DestroyWindow(game.window_);
  SDL_Quit();

  game.was_started_ = false;
}

Game& Game::GetInstance() {
  static Game instance;
  return instance;
}

} // namespace gb