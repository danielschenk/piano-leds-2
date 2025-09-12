#include "FreeRtosMonotonicTime.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

uint32_t FreeRtosMonotonicTime::getMilliseconds() const
{
    return (xTaskGetTickCount() * 1000) / configTICK_RATE_HZ;
}
