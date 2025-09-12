#include "BaseTask.hpp"

BaseTask::~BaseTask()
{
    assert(taskHandle == NULL);
}

void BaseTask::start(const char* name, uint32_t stackSize, UBaseType_t priority)
{
    assert(taskHandle == NULL);

    xTaskCreate(&BaseTask::taskFunction, name, stackSize, this, priority, &taskHandle);

    assert(taskHandle != NULL);
}

void BaseTask::terminate()
{
    terminating = true;
}

TaskHandle_t BaseTask::getTaskHandle() const
{
    return taskHandle;
}

void BaseTask::taskFunction(void* pvParameters)
{
    // pvPameters points to the instance
    BaseTask* instance(static_cast<BaseTask*>(pvParameters));

    while (!instance->terminating)
    {
        instance->run();
    }

    vTaskDelete(instance->taskHandle);
    instance->taskHandle = NULL;
}
