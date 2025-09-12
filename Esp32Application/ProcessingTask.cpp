#include "ProcessingTask.h"

#include "Concert.h"

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

    concert.execute();
}
