#ifndef ESP32APPLICATION_LOGGINGTASK_H_
#define ESP32APPLICATION_LOGGINGTASK_H_

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "BaseTask.hpp"
#include "ILoggingTarget.hpp"

class Stream;

/**
 * The logging task.
 */
class LoggingTask : public ILoggingTarget, public BaseTask
{
  public:
    /**
     * Constructor.
     *
     * @param serial    The Arduino Serial instance to use
     * @param stackSize Stack size in words
     * @param priority  Priority
     */
    LoggingTask(Stream& serial, uint32_t stackSize, UBaseType_t priority);

    /**
     * Destructor.
     */
    ~LoggingTask() override;

    // Prevent implicit constructors and assignment operator
    LoggingTask() = delete;
    LoggingTask(const LoggingTask&) = delete;
    LoggingTask& operator=(const LoggingTask&) = delete;

    // ILoggingTarget implementation
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
};

#endif /* ESP32APPLICATION_LOGGINGTASK_H_ */
