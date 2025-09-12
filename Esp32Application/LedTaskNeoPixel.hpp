#ifndef ESP32APPLICATION_LEDTASKNEOPIXEL_H_
#define ESP32APPLICATION_LEDTASKNEOPIXEL_H_

#include <mutex>

#include "BaseTask.hpp"
#include "Concert.hpp"
#include "NeoPixelBus.h"

class LedTaskNeoPixel : public BaseTask, public Concert::IObserver
{
  public:
    LedTaskNeoPixel(Concert& concert, int16_t dataPin, uint32_t stackSize, UBaseType_t priority);

    ~LedTaskNeoPixel() override;

    // Concert::IObserver implementation
    void onStripUpdate(const processing::RgbStrip& strip) override;

  private:
    // BaseTask implementation
    void run() override;

    static constexpr TickType_t autoRefreshInterval = 100;

    processing::RgbStrip pendingValues;
    NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> strip;
    mutable std::mutex mutex;
    Concert& concert;
};

#endif
