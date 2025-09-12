#ifndef ESP32APPLICATION_BASETASK_H_
#define ESP32APPLICATION_BASETASK_H_

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <atomic>

/**
 * Base class for a FreeRTOS task.
 */
class BaseTask
{
  public:
    BaseTask(const BaseTask&) = delete;
    BaseTask& operator=(const BaseTask&) = delete;

    virtual ~BaseTask();

    void terminate();

  protected:
    BaseTask() = default;

    /**
     * Create and start the task.
     *
     * @param name      Name of the task
     * @param stackSize Stack size in words
     * @param priority  Priority
     */
    void start(const char* name, uint32_t stackSize, UBaseType_t priority);

    TaskHandle_t getTaskHandle() const;

  private:
    /**
     * Run function of the task. Is repeatedly executed until termination.
     */
    virtual void run() = 0;

    /**
     * Entry point for FreeRTOS, needs to be static.
     *
     * @param pvParameters  Should point to the instance.
     */
    static void taskFunction(void* pvParameters);

    TaskHandle_t taskHandle = nullptr;
    std::atomic<bool> terminating{false};
};

#endif /* ESP32APPLICATION_BASETASK_H_ */
