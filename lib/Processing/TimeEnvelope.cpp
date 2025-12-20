#include "TimeEnvelope.hpp"

namespace processing
{

float TimeEnvelope::calculateProgress(uint32_t now, uint32_t startTime) const
{
    bool attack = (now - startTime) <= attackMs;
    float progress =
        attack ? float(now - startTime) / attackMs : float(now - startTime - attackMs) / releaseMs;
    return attack ? progress : 1 - progress;
}

uint32_t TimeEnvelope::duration() const
{
    return attackMs + releaseMs;
}

bool TimeEnvelope::completed(uint32_t now, uint32_t startTime) const
{
    return now - startTime >= duration();
}

}  // namespace processing
