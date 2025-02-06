#include "game.hpp"

#include "logger/logger.hpp"

#include "SDL.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

namespace gb {

Game::~Game() {
  Exit();
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

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    Logger::Fatal("Не удалось инициализировать SDL...");
    return;
  }

  if (game.window_ = SDL_CreateWindow("ImGui + SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE); !game.window_) {
    Logger::Fatal("Не удалось инициализировать окно...");
    return;
  }

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
  //io.Fonts->AddFontFromFileTTF("res/fonts/minecraft_seven.ttf", 16.0F, nullptr, io.Fonts->GetGlyphRangesCyrillic());

  // Инициализация ImGui для SDL2 и SDL_Renderer
  ImGui_ImplSDL2_InitForSDLRenderer(game.window_, game.renderer_);
  ImGui_ImplSDLRenderer2_Init(game.renderer_);

  Logger::Info("Инициализация игры прошла успешно!");

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

  // Начало нового кадра ImGui
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  // Создание ImGui интерфейса
  ImGui::Begin("Hello, world!");
  ImGui::Text("Это пример использования ImGui с SDL2.");
  if (ImGui::Button("Закрыть")) game.should_exit_ = true;
  ImGui::End();

  // Отрисовка
  ImGui::Render();
  SDL_SetRenderDrawColor(game.renderer_, 0, 0, 0, 255);
  SDL_RenderClear(game.renderer_);
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), game.renderer_);
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