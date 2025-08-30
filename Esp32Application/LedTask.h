#ifndef ESP32APPLICATION_LEDTASK_H_
#define ESP32APPLICATION_LEDTASK_H_

#include <mutex>
#include <Adafruit_WS2801.h>

#include "BaseTask.h"
#include "Concert.h"

/**
 * Task which performs the output to the LED strip.
 */
class LedTask
    : public BaseTask
    , public Concert::IObserver
{
public:
    /**
     * Constructor.
     *
     * @param concert   Concert instance to subscribe for LED updates and check strip size
     * @param dataPin   Pin number where the data input of the strip is connected, -1 for hardware SPI
     * @param clockPin  Pin number where the clock input of the strip is connected, -1 for hardware SPI
     * @param stackSize Stack size in words
     * @param priority  Priority
     */
    LedTask(Concert& concert,
            int16_t dataPin,
            int16_t clockPin,
            uint32_t stackSize,
            UBaseType_t priority);

    /**
     * Destructor.
     */
    ~LedTask() override;

    // Concert::IObserver implementation
    void onStripUpdate(const Processing::TRgbStrip& strip) override;

private:
    // BaseTask implementation
    void run() override;

    static constexpr TickType_t autoRefreshInterval = 100;

    /** Values stored upon last update. */
    Processing::TRgbStrip pendingValues;

    /** The LED strip driver. */
    Adafruit_WS2801 strip;

    /** Mutex protecting the pending values. */
    mutable std::mutex mutex;

    Concert& concert;
};

#endif /* ESP32APPLICATION_LEDTASK_H_ */
