#include <algorithm>

#include "LedTaskNeoPixel.h"


LedTaskNeoPixel::LedTaskNeoPixel(Concert& concert,
                 int16_t dataPin,
                 uint32_t stackSize,
                 UBaseType_t priority)
    : strip(concert.getStripSize(), dataPin)
    , concert(concert)
{
    strip.Begin();
    start("led", stackSize, priority);

    concert.subscribe(*this);
}

LedTaskNeoPixel::~LedTaskNeoPixel()
{
    concert.unsubscribe(*this);
}

void LedTaskNeoPixel::onStripUpdate(const Processing::TRgbStrip& strip)
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        // Store the new values.
        pendingValues = strip;
    }

    // Notify the task that an update was received.
    xTaskNotifyGive(getTaskHandle());
}

void LedTaskNeoPixel::run()
{
    // Wait until a strip update comes, or the auto-refresh interval has been reached.
    auto notifyGiveCount(ulTaskNotifyTake(pdTRUE, autoRefreshInterval));

    if (notifyGiveCount != 0)
    {
        // A strip update was received, update the colors in the driver.
        std::lock_guard<std::mutex> lock(mutex);

        for(unsigned int ledNumber(0); ledNumber < pendingValues.size(); ++ledNumber)
        {
            if(ledNumber >= strip.PixelCount())
                break;

            auto& color(pendingValues[ledNumber]);
            strip.SetPixelColor(ledNumber, RgbColor{color.r, color.g, color.b});
        }
    }

    // Send latest state to strip.
    // This is done outside of the lock to prevent holding up the thread sending the
    // strip updates. The driver is accessed only in this thread anyway.
    strip.Show();
}
