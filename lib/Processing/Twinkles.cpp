#include "Twinkles.hpp"

#include <cstdlib>

#include "Logging.hpp"
#include "MonotonicTime.hpp"

#define LOGGING_COMPONENT "Twinkles"

namespace processing
{

Twinkles::Twinkles(const MonotonicTime& monotonicTime) : monotonicTime(monotonicTime) {}

void Twinkles::execute(RgbStrip& strip, const NoteToLightMap& noteToLightMap)
{
    auto now = monotonicTime.getMilliseconds();

    pruneDeadTwinkles(now);
    if (now - lastOnTimeMs >= startIntervalMs)
    {
        addNewTwinkle(strip.size(), now);
        lastOnTimeMs = now;
    }

    render(strip, now);
}

static unsigned int boundedRand(unsigned int range)
{
    for (unsigned int x, r;;)
        if (x = rand(), r = x % range, x - r <= -range)
            return r;
}

void Twinkles::addNewTwinkle(std::size_t stripSize, uint32_t now)
{
    if (twinkles.size() >= stripSize)
        return;

    for (std::size_t attempt = 0; attempt < stripSize; ++attempt)
    {
        std::size_t pos = boundedRand(stripSize);
        if (twinkles.find(pos) == twinkles.end())
        {
            twinkles[pos] = now;
            break;
        }
    }
}

void Twinkles::pruneDeadTwinkles(uint32_t now)
{
    for (auto it = twinkles.cbegin(); it != twinkles.cend();)
        if (now > it->second + fadeInMs + fadeOutMs)
            twinkles.erase(it++);
        else
            ++it;
}

void Twinkles::render(RgbStrip& strip, uint32_t now)
{
    for (const auto& pair : twinkles)
    {
        auto& startTime = pair.second;
        bool fadingIn = (now - startTime) <= fadeInMs;
        float fadeProgress = fadingIn ? float(now - startTime) / fadeInMs
                                      : float(now - startTime - fadeInMs) / fadeOutMs;
        float brightness = fadingIn ? fadeProgress : 1 - fadeProgress;

        strip[pair.first] = color * brightness;
    }
}

}  // namespace processing
