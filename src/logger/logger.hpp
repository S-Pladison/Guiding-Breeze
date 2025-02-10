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
  Debug,   //< Отладочная информация, не влияющие на работу программы
  Info,    //< Сообщения, не влияющие на работу программы
  Warning, //< Предупреждения, которые могут привести к непредвиденному поведению
  Error,   //< Ошибки, которые могут привести к непредвиденному поведению
  Fatal    //< Критические ошибки, прерывающие выполнение программы
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

  if constexpr (Level == Level::Debug) {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
  }
  else if constexpr (Level == Level::Info) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
  }
  else if constexpr (Level == Level::Warning) {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
  }
  else if constexpr (Level == Level::Error) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
  }
  else if constexpr (Level == Level::Fatal) {
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
  Log<Level::Debug>(fmt, args...);
}

/**
 * @brief Функция для вывода логов на уровне INFO.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
inline void Info(const std::string& fmt, Args&&... args) {
  Log<Level::Info>(fmt, args...);
}

/**
 * @brief Функция для вывода логов на уровне WARN.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
inline void Warn(const std::string& fmt, Args&&... args) {
  Log<Level::Warning>(fmt, args...);
}

/**
 * @brief Функция для вывода логов на уровне ERROR.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
inline void Error(const std::string& fmt, Args&&... args) {
  Log<Level::Error>(fmt, args...);
}

/**
 * @brief Функция для вывода логов на уровне FATAL.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
inline void Fatal(const std::string& fmt, Args&&... args) {
  Log<Level::Fatal>(fmt, args...);
}

} // namespace gb::Logger

#endif // GUIDING_BREEZE_SRC_LOGGER_LOGGER_H