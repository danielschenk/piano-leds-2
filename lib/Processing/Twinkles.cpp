#include "Twinkles.hpp"

#include <cstdlib>

#include "Logging.hpp"

#define LOGGING_COMPONENT "Twinkles"

namespace processing
{

void Twinkles::execute(RgbStrip& strip, const Input& input)
{
    if (twinkles.size() != strip.size())
    {
        twinkles.resize(strip.size());
        twinkles.shrink_to_fit();
    }

    pruneDeadTwinkles(input.nowMs);
    if (input.nowMs - lastSpawnTimeMs >= spawnIntervalMs)
    {
        spawnTwinkle(strip.size(), input.nowMs);
        lastSpawnTimeMs = input.nowMs;
    }

    render(strip, input.nowMs);
}

void Twinkles::deactivate()
{
    twinkles.clear();
    twinkles.shrink_to_fit();
}

static unsigned int boundedRand(unsigned int range)
{
    for (unsigned int x, r;;)
        if (x = rand(), r = x % range, x - r <= -range)
            return r;
}

void Twinkles::spawnTwinkle(std::size_t stripSize, uint32_t now)
{
    for (std::size_t attempt = 0; attempt < stripSize; ++attempt)
    {
        std::size_t pos = boundedRand(stripSize);
        if (!twinkles[pos].has_value())
        {
            twinkles[pos] = Twinkle{now};
            break;
        }
    }
}

void Twinkles::pruneDeadTwinkles(uint32_t now)
{
    for (auto& twinkle : twinkles)
        if (twinkle.has_value() && envelope.completed(now, twinkle->spawnTimeMs))
            twinkle.reset();
}

void Twinkles::render(RgbStrip& strip, uint32_t now)
{
    for (std::size_t pos = 0; pos < twinkles.size(); ++pos)
    {
        auto& twinkle = twinkles[pos];
        if (!twinkle.has_value())
            continue;

        float brightness = envelope.calculateProgress(now, twinkle->spawnTimeMs);
        strip[pos] = color * brightness;
    }
}

}  // namespace processing
