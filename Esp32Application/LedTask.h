#ifndef ESP32APPLICATION_LEDTASK_H_
#define ESP32APPLICATION_LEDTASK_H_

#include <Adafruit_WS2801.h>

#include <mutex>

#include "BaseTask.h"
#include "Concert.h"

/**
 * Task which performs the output to the LED strip.
 */
class LedTask : public BaseTask, public Concert::IObserver
{
  public:
    LedTask(Concert& concert, int16_t dataPin, int16_t clockPin, uint32_t stackSize,
            UBaseType_t priority);

    ~LedTask() override;

    // Concert::IObserver implementation
    void onStripUpdate(const Processing::TRgbStrip& strip) override;

  private:
    // BaseTask implementation
    void run() override;

    static constexpr TickType_t autoRefreshInterval = 100;

    Processing::TRgbStrip pendingValues;
    Adafruit_WS2801 strip;
    mutable std::mutex mutex;
    Concert& concert;
};

#endif /* ESP32APPLICATION_LEDTASK_H_ */
