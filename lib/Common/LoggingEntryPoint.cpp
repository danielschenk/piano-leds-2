#include "LoggingEntryPoint.hpp"

#include <stdio.h>

#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <mutex>
#include <string>
#include <vector>

#include "LoggingTarget.hpp"
#include "MonotonicTime.hpp"

std::vector<LoggingTarget*> LoggingEntryPoint::subscribers;
std::mutex LoggingEntryPoint::mutex;
const MonotonicTime* LoggingEntryPoint::monotonicTime(nullptr);

void LoggingEntryPoint::subscribe(LoggingTarget& subscriber)
{
    std::lock_guard<std::mutex> lock(mutex);
    bool found(false);
    for (auto loggingTarget : subscribers)
    {
        if (loggingTarget == &subscriber)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        subscribers.push_back(&subscriber);
    }
}

void LoggingEntryPoint::unsubscribe(LoggingTarget& subscriber)
{
    std::lock_guard<std::mutex> lock(mutex);
    subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), &subscriber),
                        subscribers.end());
}

void LoggingEntryPoint::logMessage(logging::Level level, const char* component, const char* fmt,
                                   ...)
{
    assert(monotonicTime != nullptr);
    uint32_t time(monotonicTime->getMilliseconds());

    std::lock_guard<std::mutex> lock(mutex);
    if (subscribers.size() > 0)
    {
        std::vector<char> buffer(maxMessageSize);
        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer.data(), buffer.size(), fmt, args);
        va_end(args);

        std::string message(buffer.data());
        std::string componentStr(component);
        for (auto loggingTarget : subscribers)
        {
            if (loggingTarget != nullptr)
            {
                loggingTarget->logMessage(time, level, componentStr, message);
            }
        }
    }
}

void LoggingEntryPoint::setTime(const MonotonicTime* time)
{
    monotonicTime = time;
}
