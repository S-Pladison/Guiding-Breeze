#ifndef GUIDING_BREEZE_SRC_LOGGER_LOGGER_H
#define GUIDING_BREEZE_SRC_LOGGER_LOGGER_H

#include <stdexcept>
#include <string>

#include "SDL_log.h"
#include "fmt/base.h"
#include "fmt/format.h"
#include "sys/types.h"

namespace gb::Logger {

/**
 * @brief Перечисление уровней логирования.
 */
enum class Level : u_short {
  kDebug,   //< Отладочная информация, не влияющие на работу программы
  kInfo,    //< Сообщения, не влияющие на работу программы
  kWarning, //< Предупреждения, которые могут привести к непредвиденному поведению
  kError,   //< Ошибки, которые могут привести к непредвиденному поведению
  kFatal    //< Критические ошибки, прерывающие выполнение программы
};

/**
 * @brief Функция для вывода логов.
 * 
 * @param level Уровень логирования.
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 * @throw std::logic_error Если передан недопустимый уровень логирования.
 */
template<Level Level, typename... Args>
void Log(const std::string& fmt, Args&&... args) {
  auto msg = fmt::format(fmt::runtime(fmt), args...);

  if constexpr (Level == Level::kDebug) {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
  }
  else if constexpr (Level == Level::kInfo) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
  }
  else if constexpr (Level == Level::kWarning) {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
  }
  else if constexpr (Level == Level::kError) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
  }
  else if constexpr (Level == Level::kFatal) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
  }
  else {
    throw std::logic_error(
      fmt::format("Нереализованный уровень логирования: {}", static_cast<u_short>(Level))
    );
  }
 }

/**
 * @brief Функция для вывода логов на уровне DEBUG.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
inline void Debug(const std::string& fmt, Args&&... args) {
  Log<Level::kDebug>(fmt, args...);
}

/**
 * @brief Функция для вывода логов на уровне INFO.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
inline void Info(const std::string& fmt, Args&&... args) {
  Log<Level::kInfo>(fmt, args...);
}

/**
 * @brief Функция для вывода логов на уровне WARN.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
inline void Warn(const std::string& fmt, Args&&... args) {
  Log<Level::kWarning>(fmt, args...);
}

/**
 * @brief Функция для вывода логов на уровне ERROR.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
inline void Error(const std::string& fmt, Args&&... args) {
  Log<Level::kError>(fmt, args...);
}

/**
 * @brief Функция для вывода логов на уровне FATAL.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
inline void Fatal(const std::string& fmt, Args&&... args) {
  Log<Level::kFatal>(fmt, args...);
}

} // namespace gb::Logger

#endif // GUIDING_BREEZE_SRC_LOGGER_LOGGER_H