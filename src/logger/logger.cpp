#include "logger.hpp"

#include <chrono>
#include <stdexcept>

// Строка сброса цвета текста в консоли.
#define STR_RESET_COLOR "\e[0m"

// Макрос формирования строки для вывода в консоль с определенным цветом.
#define STR_COLORED_RGB(r, g, b, str) \
  STR_RESET_COLOR "\033[38;2;" #r ";" #g ";" #b "m" str STR_RESET_COLOR

namespace gb::Logger {

/**
  * @brief Получение информации косательно текущего времени в виде строки.
  *
  * @return std::string Строка с текущим временем в формате "YYYY-MM-DD HH:MM:SS".
  */
[[nodiscard]] std::string GetFormattedTime() {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  auto tm = *std::localtime(&time);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

  return oss.str();
}

/**
  * @brief Получение цветной версии строки уровня логирования для последующего вывода в консоль.
  *
  * @param level Уровень логирования.
  * @return const std::string& Строка уровня логирования.
  * @throw std::logic_error Если передан неизвестный уровень логирования.
  */
[[nodiscard]] const std::string& LevelAsColoredString(Level level) {
  switch (level) {
  case Level::Debug:
    static const std::string kDebug = STR_COLORED_RGB(30, 200, 145, "[DEBUG]");
    return kDebug;
  case Level::Info:
    static const std::string kInfo = STR_COLORED_RGB(130, 130, 130, "[INFO]");
    return kInfo;
  case Level::Warning:
    static const std::string kWarning = STR_COLORED_RGB(200, 145, 30, "[WARNING]");
    return kWarning;
  case Level::Error:
    static const std::string kError = STR_COLORED_RGB(200, 30, 70, "[ERROR]");
    return kError;
  case Level::Fatal:
    static const std::string kFatal = STR_COLORED_RGB(200, 30, 70, "[FATAL]");
    return kFatal;
  default:
    throw std::logic_error(
      fmt::format("Недопустимый уровень логирования: {}", static_cast<int>(level))
    );
  }
}

/**
  * @brief Преобразование перечисления типа SDL_LogPriority в Level.
  *
  * @param priority Значение SDL_LogPriority.
  * @return Level Соответствующий для priority LogLevel.
  * @throw std::logic_error Если priority не соответствует ни одному из значений SDL_LogPriority.
  */
[[nodiscard]] Level SDLLogPriorityToLogLevel(SDL_LogPriority priority) {
  switch (priority) {
    case SDL_LOG_PRIORITY_DEBUG:
      return Level::Debug;
    case SDL_LOG_PRIORITY_INFO:
      return Level::Info;
    case SDL_LOG_PRIORITY_WARN:
      return Level::Warning;
    case SDL_LOG_PRIORITY_ERROR:
      return Level::Error;
    case SDL_LOG_PRIORITY_CRITICAL:
      return Level::Fatal;
    default:
      throw std::logic_error(
        fmt::format("Недопустимый приоритет загрузки: {}", static_cast<int>(priority))
      );
  }
}

/**
  * @brief Структура для инициализации функции вывода логов.
  */
struct LoggerInitializer final {
public:
  LoggerInitializer() {
    SDL_LogSetOutputFunction(Output, nullptr);
  }

  ~LoggerInitializer() {
    SDL_LogSetOutputFunction(nullptr, nullptr);
  }

private:
  static void Output(void*, int, SDL_LogPriority priority, const char * message) {
    auto time_str = GetFormattedTime();
    auto level_str = LevelAsColoredString(SDLLogPriorityToLogLevel(priority));

    fmt::println("[{}] {} {}", time_str.c_str(), level_str.c_str(), message);
  }
} _;

} // namespace gb::Logger
