/**
 * @file
 *
 * MIT License
 *
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @brief Processing type definitions.
 */

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
    /**
     * Constructor.
     *
     * @param[in]   r   Initial red value.
     * @param[in]   g   Initial green value.
     * @param[in]   b   Initial blue value.
     */
    TRgb(uint8_t r, uint8_t g, uint8_t b);

    /**
     * Default constructor, initializes values to 0.
     */
    TRgb();

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

    uint8_t r;
    uint8_t g;
    uint8_t b;
};

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
