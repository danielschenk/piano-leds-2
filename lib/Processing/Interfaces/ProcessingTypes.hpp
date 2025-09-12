#ifndef PROCESSING_INTERFACES_PROCESSINGTYPES_H_
#define PROCESSING_INTERFACES_PROCESSINGTYPES_H_

#include "Color.hpp"
#include "json11.hpp"
using Json = json11::Json;

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace processing
{

struct TLinearConstants
{
    float factor;
    float offset;

    /**
     * Compare with another @ref TLinearConstants.
     */
    bool operator==(const TLinearConstants& other) const;
    bool operator!=(const TLinearConstants& other) const;
};

/** Type to map MIDI note numbers to lights. */
typedef std::map<uint8_t, uint16_t> TNoteToLightMap;

Json convert(const TNoteToLightMap& source);
TNoteToLightMap convert(const Json& source);

/** Type for actual time in milliseconds. */
typedef uint32_t TTime;

/** Type for actual note states. */
struct TNoteState
{
    TNoteState() = default;
    TNoteState(bool pressed, bool sounding, uint8_t pressDownVelocity, TTime noteOnTimeStamp);
    TNoteState(const TNoteState&) = default;

    /** Indicates if the note is pressed (true if an on event was received last, false if an off
     * event was received last). */
    bool pressed{false};
    /** Indicates if the note is sounding (either because it's pressed or the damper pedal is
     * pressed). */
    bool sounding{false};
    /** The press down velocity. */
    uint8_t pressDownVelocity{0};
    /** The time stamp of the note on event. */
    TTime noteOnTimeStamp{0};

    // TODO temp hack - this ideally shouldn't be tracked here
    RgbColor pressDownColor;
};

}  // namespace processing

#endif /* PROCESSING_INTERFACES_PROCESSINGTYPES_H_ */
