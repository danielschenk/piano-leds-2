#ifndef NETWORKTASK_H
#define NETWORKTASK_H

#include <WiFi.h>
#include <freertos/FreeRTOS.h>

#include "BaseTask.hpp"

class SystemSettingsModel;

class NetworkTask : public BaseTask
{
  public:
    /**
     * Constructor.
     *
     * @param systemSettingsModel   The system settings model to get WiFi settings from
     * @param stackSize             The stack size to use for the task, in words
     * @param priority              The priority to use for the task
     */
    NetworkTask(const SystemSettingsModel& systemSettingsModel, uint32_t stackSize,
                UBaseType_t priority);

    /**
     * Destructor.
     */
    ~NetworkTask() override;

    // Prevent default constructor, copy constructor and assignment operator
    NetworkTask() = delete;
    NetworkTask(const NetworkTask&) = delete;
    NetworkTask& operator=(const NetworkTask&) = delete;

  private:
    // BaseTask implementation
    void run() override;

    const SystemSettingsModel& systemSettingsModel;
    unsigned int systemSettingsModelSubscription;
};

#endif  // NETWORKTASK_H
