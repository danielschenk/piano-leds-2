#include "ProcessingTask.hpp"

#include "Concert.hpp"
#include "Logging.hpp"

#define LOGGING_COMPONENT "ProcessingTask"

ProcessingTask::ProcessingTask(Concert& concert, uint32_t stackSize, UBaseType_t priority)
    : BaseTask(), concert(concert), lastWakeTime(xTaskGetTickCount())
{
    start("processing", stackSize, priority);
}

ProcessingTask::~ProcessingTask() {}

void ProcessingTask::run()
{
    // Wait for the next cycle.
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(runIntervalMs));

#ifdef DIAGNOSTICS
    auto start = esp_timer_get_time();
#endif
    concert.execute();
#ifdef DIAGNOSTICS
    auto dur_us = esp_timer_get_time() - start;
    if (dur_us > maxExecUs)
        maxExecUs = (uint32_t)dur_us;
    cycles++;
    // Detect overrun if the execution time exceeds the interval budget significantly
    if (dur_us > (runIntervalMs * 1000))
        overrunCount++;
    if ((cycles % 3000) == 0)
    {
        UBaseType_t hwm = uxTaskGetStackHighWaterMark(NULL);
        LOG_INFO_PARAMS("PROC diag: cycles=%lu maxExec_us=%lu overrun=%lu stackHWM=%lu",
                        (unsigned long)cycles, (unsigned long)maxExecUs,
                        (unsigned long)overrunCount, (unsigned long)hwm);
    }
#endif
}
