#pragma once

#include <QDebug>
#include <QString>
#include <cstdarg>

namespace TerminalColors
{
constexpr const char *RED = "\033[0;31m";
constexpr const char *CYN = "\033[0;36m";
constexpr const char *YEL = "\033[0;33m";
constexpr const char *RES = "\033[0m";
} // namespace TerminalColors

class Logger
{
  public:
    static void logError(const char *fmt, ...);
    static void logWarning(const char *fmt, ...);
    static void logInfo(const char *fmt, ...);

  private:
    static QString formatMessage(const char *fmt, va_list args);
};
