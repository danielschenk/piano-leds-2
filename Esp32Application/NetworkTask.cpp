#include "NetworkTask.h"

#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "SystemSettingsModel.h"

NetworkTask::NetworkTask(const SystemSettingsModel &systemSettingsModel, uint32_t stackSize,
                         UBaseType_t priority)
    : BaseTask(), systemSettingsModel(systemSettingsModel)
{
    start("network", stackSize, priority);
}

NetworkTask::~NetworkTask() {}

void NetworkTask::run()
{
    // TODO scan & try connect to known network before enabling AP

    std::string apSsid(systemSettingsModel.getWifiAPSsid());
    if (!apSsid.empty())
    {
        WiFi.softAPConfig({192, 168, 1, 1}, {192, 168, 1, 1}, {255, 255, 255, 0});
        WiFi.softAP(apSsid.c_str(), systemSettingsModel.getWifiAPPassword().c_str());
    }

    // Wait for event
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}
