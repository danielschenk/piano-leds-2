#ifndef PROCESSING_INTERFACES_PROCESSINGTYPES_H_
#define PROCESSING_INTERFACES_PROCESSINGTYPES_H_

#include "json11.hpp"
using Json = json11::Json;

#include <cstdint>
#include <map>
#include <vector>
#include <string>

namespace Processing
{

/** Type for single RGB color. */
struct TRgb
{
    constexpr TRgb(uint8_t r, uint8_t g, uint8_t b)
        : r(r)
        , g(g)
        , b(b)
    {
    }

    constexpr TRgb() = default;

    /**
     * Compare with another @ref TRgb.
     */
    bool operator==(const TRgb& other) const;
    bool operator!=(const TRgb& other) const;

    /**
     * Multiply every color by a single factor.
     */
    TRgb operator*(float factor) const;

    /**
     * Add colors together.
     */
    TRgb operator+(const TRgb& other) const;

    /**
     * Subtract colors from each other.
     */
    TRgb operator-(const TRgb& other) const;

    /**
     * Assign colors.
     */
    TRgb& operator=(const TRgb& other) = default;

    /**
     * Add colors together and assign the result to this object.
     */
    TRgb& operator+=(const TRgb& other);

    /**
     * Subtract colors from each other and assign the result to this object.
     */
    TRgb& operator-=(const TRgb& other);

    // Implements custom value printing for Google Test
    friend std::ostream& operator<<(std::ostream& os, const TRgb& color);

    uint8_t r{0};
    uint8_t g{0};
    uint8_t b{0};
};

namespace ColorValue
{
    constexpr TRgb off{};
    constexpr TRgb red{0xff, 0x00, 0x00};
    constexpr TRgb green{0x00, 0xff, 0x00};
    constexpr TRgb blue{0x00, 0x00, 0xff};
    constexpr TRgb yellow{0xff, 0xff, 0x00};
    constexpr TRgb magenta{0xff, 0x00, 0xff};
    constexpr TRgb cyan{0x00, 0xff, 0xff};
    constexpr TRgb white{0xff, 0xff, 0xff};
}

/**
 * Multiply every color by a single factor.
 */
TRgb operator*(float factor, const TRgb& color);

/**
 * Construct color from float values, clamping if necessary.
 */
TRgb rgbFromFloat(float initialR, float initialG, float initialB);

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

/** Type for RGB strip data. */
typedef std::vector<TRgb> TRgbStrip;

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

    /** Indicates if the note is pressed (true if an on event was received last, false if an off event was received last). */
    bool pressed{false};
    /** Indicates if the note is sounding (either because it's pressed or the damper pedal is pressed). */
    bool sounding{false};
    /** The press down velocity. */
    uint8_t pressDownVelocity{0};
    /** The time stamp of the note on event. */
    TTime noteOnTimeStamp{0};
};

} /* namespace Processing */


#endif /* PROCESSING_INTERFACES_PROCESSINGTYPES_H_ */
