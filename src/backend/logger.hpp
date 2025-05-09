/**
 * @file logger.hpp
 * @brief Defines the Logger class, the messageHandler method and TerminalColors namespace for logging with colored
 * output.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 03-04-2025
 */

#pragma once

#include <QDebug>

/**
 * @namespace TerminalColors
 *
 * @brief Provides ANSI escape codes for terminal text coloring.
 */
namespace TerminalColors {
/** @brief Red color for critical or error messages. */
constexpr const char *RED = "\033[0;31m";

/** @brief Yellow color for warning messages. */
constexpr const char *YEL = "\033[0;33m";

/** @brief Blue color for debug messages. */
constexpr const char *BLU = "\033[0;34m";

/** @brief Cyan color for info messages. */
constexpr const char *CYN = "\033[0;36m";

/** @brief Reset color to default terminal color. */
constexpr const char *RES = "\033[0m";
}  // namespace TerminalColors

/**
 * @class Logger
 *
 * @brief Handles logging with colored output for different log levels.
 */
class Logger {
   public:
    /**
     * @brief Custom message handler for Qt logging.
     *
     * @param type The type of the message.
     * @param context The context of the message.
     * @param message The actual log message to be formatted and displayed.
     */
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);
};