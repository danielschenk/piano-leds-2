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

void StdLogger::logMessage(uint64_t time, logging::Level level, std::string component,
                           std::string message)
{
    const char* levelString;
    switch (level)
    {
        case logging::Level::debug:
            levelString = "Debug";
            break;

        case logging::Level::info:
            levelString = "Info";
            break;

        case logging::Level::warning:
            levelString = "Warning";
            break;

        case logging::Level::error:
        default:
            levelString = "Error";
            break;
    }

    std::cout << time << " " << levelString << "(" << component << "):" << message << "\r\n";
}
