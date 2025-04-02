#include "logger.hpp"

void Logger::logError(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    QString message = formatMessage(fmt, args);
    fprintf(stderr, "%s[Error] %s%s\n", TerminalColors::RED, TerminalColors::RES, message.toUtf8().constData());

    va_end(args);
}

void Logger::logWarning(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    QString message = formatMessage(fmt, args);
    fprintf(stderr, "%s[Warning] %s%s\n", TerminalColors::YEL, TerminalColors::RES, message.toUtf8().constData());

    va_end(args);
}

void Logger::logInfo(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    QString message = formatMessage(fmt, args);
    fprintf(stderr, "%s[Info] %s%s\n", TerminalColors::CYN, TerminalColors::RES, message.toUtf8().constData());

    va_end(args);
}

QString Logger::formatMessage(const char *fmt, va_list args)
{
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    return QString(buffer);
}
