#include "LoggingTask.hpp"

#include <Stream.h>

#include <cstdio>

#include "LoggingEntryPoint.hpp"

LoggingTask::LoggingTask(Stream& serial, uint32_t stackSize, UBaseType_t priority)
    : BaseTask(), serial(serial)
{
    queue = xQueueCreate(10, sizeof(QueueEntry));
    start("logging", stackSize, priority);
    LoggingEntryPoint::subscribe(*this);
}

LoggingTask::~LoggingTask()
{
    LoggingEntryPoint::unsubscribe(*this);
    vQueueDelete(queue);
}

void LoggingTask::logMessage(uint64_t time, Logging::TLogLevel level, std::string component,
                             std::string message)
{
    QueueEntry entry;
    entry.time = time;
    entry.level = level;
    entry.component = new std::string(component);
    entry.message = new std::string(message);

    xQueueSend(queue, &entry, portMAX_DELAY);
}

void LoggingTask::run()
{
    QueueEntry entry;

    // Wait for item forever
    while (xQueueReceive(queue, &entry, portMAX_DELAY) == pdTRUE)
    {
        // Some extra for component and level information
        char buf[LoggingEntryPoint::maxMessageSize + 100];

        const char* levelString;
        switch (entry.level)
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

        snprintf(buf, sizeof(buf), "%llu %s(%s): %s\r\n", entry.time, levelString,
                 entry.component->c_str(), entry.message->c_str());

        delete entry.component;
        delete entry.message;

        serial.print(buf);
    }
}
