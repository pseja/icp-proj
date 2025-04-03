/**
 * @file logger.cpp
 * @brief Implements the Logger class for logging with colored output.
 *
 * This file contains the implementation of the Logger class, including the
 * custom message handler for Qt logging. The message handler formats log
 * messages with appropriate colors and prefixes based on the log level
 * (e.g., Debug, Info, Warning, Critical).
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 03-04-2025
 */
#include "logger.hpp"

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    QString formattedMessage;

    switch (type)
    {
    case QtDebugMsg:
        formattedMessage = QString("%1[Debug]%2 %3").arg(TerminalColors::BLU, TerminalColors::RES, message);
        break;
    case QtInfoMsg:
        formattedMessage = QString("%1[Info]%2 %3").arg(TerminalColors::CYN, TerminalColors::RES, message);
        break;
    case QtWarningMsg:
        formattedMessage = QString("%1[Warning]%2 %3").arg(TerminalColors::YEL, TerminalColors::RES, message);
        break;
    case QtCriticalMsg:
        formattedMessage = QString("%1[Error]%2 %3").arg(TerminalColors::RED, TerminalColors::RES, message);
        break;
    }

    fprintf(stdout, "%s\n", formattedMessage.toUtf8().constData());
}
