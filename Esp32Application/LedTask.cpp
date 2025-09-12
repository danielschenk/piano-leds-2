#include "LedTask.h"

#include <algorithm>

LedTask::LedTask(Concert& concert, int16_t dataPin, int16_t clockPin, uint32_t stackSize,
                 UBaseType_t priority)
    : pendingValues(), strip(concert.getStripSize()), concert(concert)
{
    if ((dataPin > -1) && (clockPin > -1))
    {
        strip.updatePins(static_cast<uint8_t>(dataPin), static_cast<uint8_t>(clockPin));
    }
    else
    {
        strip.updatePins();
    }
    strip.begin();

    start("led", stackSize, priority);

    concert.subscribe(*this);
}

LedTask::~LedTask()
{
    concert.unsubscribe(*this);
}

void LedTask::onStripUpdate(const Processing::TRgbStrip& strip)
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        // Store the new values.
        pendingValues = strip;
    }

    // Notify the task that an update was received.
    xTaskNotifyGive(getTaskHandle());
}

void LedTask::run()
{
    // Wait until a strip update comes, or the auto-refresh interval has been reached.
    auto notifyGiveCount(ulTaskNotifyTake(pdTRUE, autoRefreshInterval));

    if (notifyGiveCount != 0)
    {
        // A strip update was received, update the colors in the driver.
        std::lock_guard<std::mutex> lock(mutex);

        for (unsigned int ledNumber(0); ledNumber < pendingValues.size(); ++ledNumber)
        {
            if (ledNumber >= strip.numPixels())
            {
                break;
            }

            auto color(pendingValues[ledNumber]);
            strip.setPixelColor(ledNumber, color.r, color.g, color.b);
        }
    }

    // Send latest state to strip.
    // This is done outside of the lock to prevent holding up the thread sending the
    // strip updates. The driver is accessed only in this thread anyway.
    strip.show();
}
