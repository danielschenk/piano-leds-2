#ifndef ESP32APPLICATION_LOGGINGTASK_H_
#define ESP32APPLICATION_LOGGINGTASK_H_

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "BaseTask.hpp"
#include "LoggingTarget.hpp"

class Stream;

class LoggingTask : public LoggingTarget, public BaseTask
{
  public:
    LoggingTask(Stream& serial, uint32_t stackSize, UBaseType_t priority);
    ~LoggingTask() override;

    LoggingTask() = delete;
    LoggingTask(const LoggingTask&) = delete;
    LoggingTask& operator=(const LoggingTask&) = delete;

    // LoggingTarget implementation
    void logMessage(uint64_t time, logging::Level level, std::string component,
                    std::string message) override;

  private:
    struct QueueEntry
    {
        uint64_t time;
        logging::Level level;
        std::string* component;
        std::string* message;
    };

    void run() override;

    Stream& serial;
    QueueHandle_t queue;
#ifdef DIAG_LIGHT
    uint32_t queueDrops{0};
#endif
};

#endif /* ESP32APPLICATION_LOGGINGTASK_H_ */
