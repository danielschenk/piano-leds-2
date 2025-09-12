#include "PianoDecayRgbFunction.hpp"

#include <json11.hpp>

processing::RgbColor PianoDecayRgbFunction::calculate(const processing::TNoteState& noteState,
                                                      processing::TTime currentTime) const
{
    auto startColor(LinearRgbFunction::calculate(noteState, currentTime));
    if (startColor == processing::RgbColor{0, 0, 0})
        return startColor;

    uint32_t soundingTime(currentTime - noteState.noteOnTimeStamp);

    float timeProgress, decayFactor, startIntensityFactor;
    if (soundingTime < fastDecayDurationMs)
    {
        timeProgress = static_cast<float>(soundingTime) / static_cast<float>(fastDecayDurationMs);
        decayFactor = fastDecayFactor;
        startIntensityFactor = 1.0f;
    }
    else
    {
        // Note timeProgress starts again from 0 here, so 0 means at 1200ms from note press down
        // time!
        timeProgress = static_cast<float>(soundingTime - fastDecayDurationMs) /
                       static_cast<float>(slowDecayDurationMs);
        decayFactor = slowDecayFactor;
        // We start at the intensity factor where we left off in the first phase
        startIntensityFactor = 1.0f - fastDecayFactor;
    }

    float intensityFactor(startIntensityFactor - (timeProgress * decayFactor));
    return startColor * intensityFactor;
}

std::string PianoDecayRgbFunction::getObjectType() const
{
    return IRgbFunction::jsonTypeNamePianoDecayRgbFunction;
}
