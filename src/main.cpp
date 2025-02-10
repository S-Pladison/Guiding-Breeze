#include "logger/logger.hpp"

#include <cstdlib>

#include "SDL.h"
#include "SDL_events.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

//< Реализации данных функций находятся в src/core/game.cpp
namespace gb {

extern bool IsExitRequested(); //< Функция проверки запроса на завершение игры
extern void RequestExit(); //< Функция запроса на завершения игры

extern void OnStart(SDL_Window* window, SDL_Renderer* renderer); //< Функция начала игры; Вызывается лишь раз при удачном запуске программы
extern void Update(); //< Зацикленная функция обновления логики игры
extern void Render(); //< Зацикленная функция отрисовки игры
extern void OnExit(); //< Функция завершения игры; Вызывается лишь раз перед выходом из программы

} // namespace gb

int main(int, char**) {
  gb::Logger::Info("Подготовка перед запуском игры.");

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    gb::Logger::Fatal("Не удалось инициализировать SDL...");
    return EXIT_FAILURE;
  }

  auto* window = SDL_CreateWindow("Guiding Breeze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
  if (!window) {
    gb::Logger::Fatal("Не удалось инициализировать окно...");
    return EXIT_FAILURE;
  }

  auto* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    gb::Logger::Fatal("Не удалось инициализировать средство визуализации...");
    SDL_DestroyWindow(window);
    return EXIT_FAILURE;
  }

  IMGUI_CHECKVERSION();
  if (!ImGui::CreateContext()) {
    gb::Logger::Fatal("Не удалось инициализировать контекст ImGui...");
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return EXIT_FAILURE;
  }

  // Настройка бэкенда
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);

  // Убираем сохранение данных ImGui и добавляем шрифт
  auto& io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.Fonts->AddFontFromFileTTF("res/fonts/minecraft_seven.ttf", 16.0F, nullptr, io.Fonts->GetGlyphRangesCyrillic());

  gb::OnStart(window, renderer);

  // Основной цикл
  while (!gb::IsExitRequested()) {

    // Обработка событий SDL
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event); 
      switch (event.type) {
        case SDL_EventType::SDL_QUIT:
          gb::RequestExit();
          break;
        default:
          break;
      }
    }

    // Обновление логики игры
    gb::Update();

    // Начало кадра отрисовки
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Отрисовка кадра
    gb::Render();
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
  }

  gb::OnExit();

  // Очистка ресурсов
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return EXIT_SUCCESS;
}