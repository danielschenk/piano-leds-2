#include <vector>
#include <string>
#include <mutex>
#include <cstdarg>
#include <algorithm>
#include <cassert>
#include <stdio.h>

#include "LoggingEntryPoint.h"
#include "ILoggingTarget.h"
#include "ITime.h"

std::vector<ILoggingTarget*> LoggingEntryPoint::s_subscribers;
std::mutex LoggingEntryPoint::s_mutex;
const ITime* LoggingEntryPoint::s_time(nullptr);

void LoggingEntryPoint::subscribe(ILoggingTarget& subscriber)
{
    std::lock_guard<std::mutex> lock(s_mutex);
    bool found(false);
    for(auto loggingTarget : s_subscribers)
    {
        if(loggingTarget == &subscriber)
        {
            found = true;
            break;
        }
    }
    if(!found)
    {
        s_subscribers.push_back(&subscriber);
    }
}

void LoggingEntryPoint::unsubscribe(ILoggingTarget& subscriber)
{
    std::lock_guard<std::mutex> lock(s_mutex);
    s_subscribers.erase(std::remove(s_subscribers.begin(), s_subscribers.end(), &subscriber), s_subscribers.end());
}

void LoggingEntryPoint::logMessage(Logging::TLogLevel level, const char *component, const char *fmt, ...)
{
    assert(s_time != nullptr);
    uint32_t time(s_time->getMilliseconds());

    std::lock_guard<std::mutex> lock(s_mutex);
    if(s_subscribers.size() > 0)
    {
        std::vector<char> buffer(maxMessageSize);
        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer.data(), buffer.size(), fmt, args);
        va_end(args);

        std::string message(buffer.data());
        std::string componentStr(component);
        for(auto loggingTarget : s_subscribers)
        {
            if (loggingTarget != nullptr)
            {
                loggingTarget->logMessage(time, level, componentStr, message);
            }
        }
    }
}

void LoggingEntryPoint::setTime(const ITime* time)
{
    s_time = time;
}
