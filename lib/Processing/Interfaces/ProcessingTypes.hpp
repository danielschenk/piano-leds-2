#ifndef PROCESSING_INTERFACES_PROCESSINGTYPES_H_
#define PROCESSING_INTERFACES_PROCESSINGTYPES_H_

#include "Color.hpp"
#include "MidiInterface.hpp"
#include "json11.hpp"
using Json = json11::Json;

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace processing
{

struct LinearConstants
{
    float factor;
    float offset;

    /**
     * Compare with another @ref LinearConstants.
     */
    bool operator==(const LinearConstants& other) const;
    bool operator!=(const LinearConstants& other) const;
};

/** Type to map MIDI note numbers to lights. */
typedef std::map<uint8_t, uint16_t> NoteToLightMap;

Json convert(const NoteToLightMap& source);
NoteToLightMap convert(const Json& source);

/** Type for actual time in milliseconds. */
typedef uint32_t Timestamp;

/** Type for actual note states. */
struct NoteState
{
    NoteState() = default;
    NoteState(bool pressed, bool sounding, uint8_t pressDownVelocity, Timestamp noteOnTimeStamp);
    NoteState(const NoteState&) = default;

    /** Indicates if the note is pressed (true if an on event was received last, false if an off
     * event was received last). */
    bool pressed{false};
    /** Indicates if the note is sounding (either because it's pressed or the damper pedal is
     * pressed). */
    bool sounding{false};
    MidiInterface::Velocity pressDownVelocity{0};
    /** The time stamp of the note on event. */
    Timestamp noteOnTimeStamp{0};

    // TODO temp hack - this ideally shouldn't be tracked here
    RgbColor pressDownColor;
};
using NoteStates = std::array<processing::NoteState, MidiInterface::numNotes>;

}  // namespace processing

#endif /* PROCESSING_INTERFACES_PROCESSINGTYPES_H_ */
