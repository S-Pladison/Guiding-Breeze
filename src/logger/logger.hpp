#ifndef GUIDING_BREEZE_SRC_LOGGER_LOGGER_H
#define GUIDING_BREEZE_SRC_LOGGER_LOGGER_H

#include <string>

namespace gb::Logger {

enum class Level {
  kDebug,
  kInfo,
  kWarning,
  kError,
  kFatal
};

void Debug(const std::string& message);
void Info(const std::string& message);
void Warn(const std::string& message);
void Error(const std::string& message);
void Fatal(const std::string& message);

} // namespace gb::alog

#endif // GUIDING_BREEZE_SRC_LOGGER_LOGGER_H