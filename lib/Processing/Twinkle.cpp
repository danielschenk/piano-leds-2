#include "Twinkle.hpp"

#include <cstdlib>

#include "MonotonicTime.hpp"

namespace processing
{

Twinkle::Twinkle(const MonotonicTime& monotonicTime) : monotonicTime(monotonicTime) {}

void Twinkle::execute(RgbStrip& strip, const NoteToLightMap& noteToLightMap)
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

void Twinkle::addNewTwinkle(std::size_t stripSize, uint32_t now)
{
    std::size_t pos;
    do
        pos = boundedRand(stripSize);
    while (twinkles.find(pos) == twinkles.end());

    twinkles[pos] = now;
}

void Twinkle::pruneDeadTwinkles(uint32_t now)
{
    for (auto it = twinkles.cbegin(); it != twinkles.cend();)
        if (now > it->second + fadeInMs + fadeOutMs)
            twinkles.erase(it++);
        else
            ++it;
}

void Twinkle::render(RgbStrip& strip, uint32_t now)
{
    for (const auto& pair : twinkles)
    {
        auto& startTime = pair.second;
        bool fadingIn = (now - startTime) <= fadeInMs;
        float fadeProgress = fadingIn ? float(now - startTime) / fadeInMs
                                      : float(now - startTime + fadeInMs) / fadeOutMs;
        float brightness = fadingIn ? fadeProgress : 1 - fadeProgress;

        strip[pair.first] = color * brightness;
    }
}

}  // namespace processing
