#include "logger.hpp"

#include <chrono>
#include <iostream>
#include <stdexcept>

#include "SDL_log.h"
#include "fmt/base.h"
#include "fmt/format.h"

#define STR_RESET_COLOR "\e[0m"
#define STR_COLORED_RGB(r, g, b, str) STR_RESET_COLOR "\033[38;2;" #r ";" #g ";" #b "m" str STR_RESET_COLOR

namespace gb::Logger {

namespace {
  std::string GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
  }

  const std::string& LevelAsColoredString(Level level) {
    switch (level) {
      case Level::kDebug:
        static const std::string kDebug   = STR_COLORED_RGB(30 , 200, 145, "[DEBUG]"  );
        return kDebug;
      case Level::kInfo:
        static const std::string kInfo    = STR_COLORED_RGB(130, 130, 130, "[INFO]"   );
        return kInfo;
      case Level::kWarning:
        static const std::string kWarning = STR_COLORED_RGB(200, 145,  30, "[WARNING]");
        return kWarning;
      case Level::kError:
        static const std::string kError   = STR_COLORED_RGB(200,  30,  70, "[ERROR]"  );
        return kError;
      case Level::kFatal:
        static const std::string kFatal   = STR_COLORED_RGB(200,  30,  70, "[FATAL]"  );
        return kFatal;
      default:
        throw std::logic_error("Unknown log level...");
    }
  }

  Level SDLLogPriorityToLogLevel(SDL_LogPriority priority) {
    switch (priority) {
      case SDL_LOG_PRIORITY_DEBUG:
        return Level::kDebug;
      case SDL_LOG_PRIORITY_INFO:
        return Level::kInfo;
      case SDL_LOG_PRIORITY_WARN:
        return Level::kWarning;
      case SDL_LOG_PRIORITY_ERROR:
        return Level::kError;
      case SDL_LOG_PRIORITY_CRITICAL:
        return Level::kFatal;
      default:
        throw std::logic_error("Unknown log priority...");
    }
  }

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
      auto time_str = GetCurrentTime();
      auto level_str = LevelAsColoredString(SDLLogPriorityToLogLevel(priority));
      auto formated_msg = fmt::format("[{}] {} {}", time_str.c_str(), level_str.c_str(), message);

      std::cout << formated_msg << '\n';
    }
  } _;

} // namespace

void Log(Level level, const std::string& message) {
  switch (level) {
    case Level::kDebug:
      Debug(message);
      break;
    case Level::kInfo:
      Info(message);
      break;
    case Level::kWarning:
      Warn(message);
      break;
    case Level::kError:
      Error(message);
      break;
    case Level::kFatal:
      Fatal(message);
      break;
    default:
      throw std::logic_error("Unknown log level...");
  }
}

void Debug(const std::string& message) {
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
}

void Info(const std::string& message) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
}

void Warn(const std::string& message) {
  SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
}

void Error(const std::string& message) {
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
}

void Fatal(const std::string& message) {
  SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
}

} // namespace alog