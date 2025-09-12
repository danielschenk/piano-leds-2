#include "ProcessingTypes.hpp"

#include <cstdint>
#include <cstdio>

#include "Json11Helper.hpp"

namespace processing
{

bool TLinearConstants::operator==(const TLinearConstants& other) const
{
    return (factor == other.factor) && (offset == other.offset);
}

bool TLinearConstants::operator!=(const TLinearConstants& other) const
{
    return !(other == *this);
}

Json convert(const TNoteToLightMap& source)
{
    Json::object converted;
    for (const auto& pair : source)
    {
        char buf[4];
        snprintf(buf, sizeof(buf), "%u", pair.first);
        converted[std::string(buf)] = Json(pair.second);
    }

    return Json(converted);
}

TNoteToLightMap convert(const Json& source)
{
    Json11Helper helper(__PRETTY_FUNCTION__, source, false /* logMissingKeys */);

    TNoteToLightMap converted;
    for (unsigned int noteNumber = 0; noteNumber <= UINT8_MAX; ++noteNumber)
    {
        char buf[4];
        snprintf(buf, sizeof(buf), "%u", noteNumber);

        uint8_t lightNumber;
        if (helper.getItemIfPresent(std::string(buf), lightNumber))
        {
            converted[noteNumber] = lightNumber;
        }
    }

    return converted;
}

TNoteState::TNoteState(bool pressed, bool sounding, uint8_t pressDownVelocity,
                       TTime noteOnTimeStamp)
    : pressed(pressed),
      sounding(sounding),
      pressDownVelocity(pressDownVelocity),
      noteOnTimeStamp(noteOnTimeStamp)
{
}

}  // namespace processing
