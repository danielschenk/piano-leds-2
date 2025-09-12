#include "StdLogger.hpp"

#include <iostream>

#include "LoggingEntryPoint.hpp"

StdLogger::StdLogger()
{
    LoggingEntryPoint::subscribe(*this);
}

StdLogger::~StdLogger()
{
    LoggingEntryPoint::unsubscribe(*this);
}

void StdLogger::logMessage(uint64_t time, Logging::TLogLevel level, std::string component,
                           std::string message)
{
    const char* levelString;
    switch (level)
    {
        case Logging::LogLevel_Debug:
            levelString = "Debug";
            break;

        case Logging::LogLevel_Info:
            levelString = "Info";
            break;

        case Logging::LogLevel_Warning:
            levelString = "Warning";
            break;

        case Logging::LogLevel_Error:
        default:
            levelString = "Error";
            break;
    }

    std::cout << time << " " << levelString << "(" << component << "):" << message << "\r\n";
}
