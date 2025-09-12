#include "StripChangeLogger.h"

#include <cstdio>
#include <string>

#include "Logging.h"

#define LOGGING_COMPONENT "StripChangeLogger"

StripChangeLogger::StripChangeLogger(Concert& concert) : concert(concert), previous()
{
    concert.subscribe(*this);
}

StripChangeLogger::~StripChangeLogger()
{
    concert.unsubscribe(*this);
}

void StripChangeLogger::onStripUpdate(const Processing::TRgbStrip& strip)
{
    bool log(false);

    {
        std::lock_guard<std::mutex> lock(mutex);

        if (strip != previous)
        {
            previous = strip;
            log = true;
        }
    }

    if (log)
    {
        std::string msg("Strip update:\r\n");
        uint16_t ledNumber(0);
        for (auto led : strip)
        {
            // Fits: nnn: rrr ggg bbb[CR][LF][NUL]
            char buf[19];
            snprintf(buf, sizeof(buf), "%3u: %3u %3u %3u\r\n", ledNumber, led.r, led.g, led.b);
            msg.append(buf);
            ++ledNumber;
        }

        LOG_DEBUG_PARAMS("%s", msg.c_str());
    }
}
