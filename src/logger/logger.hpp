#ifndef GUIDING_BREEZE_SRC_LOGGER_LOGGER_H
#define GUIDING_BREEZE_SRC_LOGGER_LOGGER_H

#include <stdexcept>
#include <string>

#include "SDL_log.h"
#include "fmt/base.h"
#include "fmt/format.h"

namespace gb::Logger {

/**
 * @brief Перечисление уровней логирования.
 */
enum class Level {
  kDebug,   //< Отладочная информация, не влияющие на работу программы
  kInfo,    //< Сообщения, не влияющие на работу программы
  kWarning, //< Предупреждения, которые могут привести к непредвиденному поведению
  kError,   //< Ошибки, которые могут привести к непредвиденному поведению
  kFatal    //< Критические ошибки, прерывающие выполнение программы
};

/**
 * @brief Функция для вывода логов на уровне DEBUG.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
void Debug(const std::string& fmt, Args&&... args) {
  auto msg = fmt::format(fmt::runtime(fmt), args...);
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
}

/**
 * @brief Функция для вывода логов на уровне INFO.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
void Info(const std::string& fmt, Args&&... args) {
  auto msg = fmt::format(fmt::runtime(fmt), args...);
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
}

/**
 * @brief Функция для вывода логов на уровне WARN.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
void Warn(const std::string& fmt, Args&&... args) {
  auto msg = fmt::format(fmt::runtime(fmt), args...);
  SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
}

/**
 * @brief Функция для вывода логов на уровне ERROR.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
void Error(const std::string& fmt, Args&&... args) {
  auto msg = fmt::format(fmt::runtime(fmt), args...);
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
}

/**
 * @brief Функция для вывода логов на уровне FATAL.
 * 
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 */
template<typename... Args>
void Fatal(const std::string& fmt, Args&&... args) {
  auto msg = fmt::format(fmt::runtime(fmt), args...);
  SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", msg.c_str());
}

/**
 * @brief Функция для вывода логов.
 * 
 * @param level Уровень логирования.
 * @param fmt Сообщение для вывода.
 * @param args Аргументы для форматирования сообщения.
 * @throw std::logic_error Если передан недопустимый уровень логирования.
 */
template<typename... Args>
void Log(Level level, const std::string& fmt, Args&&... args) {
  switch (level) {
    case Level::kDebug:
      Debug(fmt, args...);
      break;
    case Level::kInfo:
      Info(fmt, args...);
      break;
    case Level::kWarning:
      Warn(fmt, args...);
      break;
    case Level::kError:
      Error(fmt, args...);
      break;
    case Level::kFatal:
      Fatal(fmt, args...);
      break;
    default:
      throw std::logic_error(
        fmt::format("Недопустимый уровень логирования: {}", static_cast<int>(level))
      );
  }
}

} // namespace gb::Logger

#endif // GUIDING_BREEZE_SRC_LOGGER_LOGGER_H