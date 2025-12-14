#ifndef BUILD_PROCESSINGTASK_H_
#define BUILD_PROCESSINGTASK_H_

#include "BaseTask.hpp"

class Concert;

/**
 * Task which executes all the processing in a @ref Concert.
 */
class ProcessingTask : public BaseTask
{
  public:
    /**
     * Constructor.
     *
     * @param concert   The concert to use
     * @param stackSize Stack size in words
     * @param priority  Priority
     */
    ProcessingTask(Concert& concert, uint32_t stackSize, UBaseType_t priority);

    /**
     * Destructor.
     */
    ~ProcessingTask() override;

  protected:
    // BaseTask implementation
    void run() override;

  private:
    static constexpr uint32_t runIntervalMs = 10;
    Concert& concert;
    TickType_t lastWakeTime;
#ifdef DIAG_LIGHT
    // Diagnostics counters
    uint32_t cycles{0};
    uint32_t overrunCount{0};
    uint32_t maxExecUs{0};
#endif
};

#endif /* BUILD_PROCESSINGTASK_H_ */
