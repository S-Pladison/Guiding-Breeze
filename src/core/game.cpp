#include "game.hpp"

#include <iostream>

#include "SDL.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

Game::~Game() {
  Exit();
}

bool Game::IsRunning() const {
  return was_started_;
}

void Game::Start() {
  if (was_started_) {
    return;
  }

  std::cout << "Game is starting..." << std::endl;

  // Инициализация SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("Ошибка: %s\n", SDL_GetError());
    return;
  }

  // Создание окна SDL
  window_ = SDL_CreateWindow("ImGui + SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  // Инициализация ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.IniFilename = nullptr;

  // Исправление кириллицы
  io.Fonts->AddFontFromFileTTF("res/fonts/minecraft_seven.ttf", 16.0F, nullptr, io.Fonts->GetGlyphRangesCyrillic());

  // Инициализация ImGui для SDL2 и SDL_Renderer
  ImGui_ImplSDL2_InitForSDLRenderer(window_, renderer_);
  ImGui_ImplSDLRenderer2_Init(renderer_);

  was_started_ = true;
}

void Game::Update() {
  if (!was_started_) {
    return;
  }

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT) should_exit_ = true;
  }

  // Начало нового кадра ImGui
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  // Создание ImGui интерфейса
  ImGui::Begin("Hello, world!");
  ImGui::Text("Это пример использования ImGui с SDL2.");
  if (ImGui::Button("Закрыть")) should_exit_ = true;
  ImGui::End();

  // Отрисовка
  ImGui::Render();
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderClear(renderer_);
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer_);
  SDL_RenderPresent(renderer_);

  if (should_exit_) {
    Exit();
  }
}

void Game::Exit() {
  if (!was_started_) {
    return;
  }

  // Очистка
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();

  std::cout << "Game is finished..." << std::endl;

  was_started_ = false;
}