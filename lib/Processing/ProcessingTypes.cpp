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
 * @brief Processing type implementations.
 */

#include "ProcessingTypes.h"
#include "Json11Helper.h"

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <iomanip>

namespace Processing
{

TRgb::TRgb(uint8_t r, uint8_t g, uint8_t b)
{
    this->r = r;
    this->g = g;
    this->b = b;
};

TRgb::TRgb()
{
    r = 0;
    g = 0;
    b = 0;
};

bool TRgb::operator==(const TRgb& other) const
{
    return (other.r == r) &&
           (other.g == g) &&
           (other.b == b);
}

bool TRgb::operator!=(const TRgb& other) const
{
    return !(other == *this);
}

TRgb TRgb::operator*(float factor) const
{
    return factor * (*this);
}

TRgb operator*(float factor, const TRgb& color)
{
    float newR(factor * (float)color.r),
          newG(factor * (float)color.g),
          newB(factor * (float)color.b);

    return rgbFromFloat(newR, newG, newB);
}

TRgb TRgb::operator+(const TRgb& other) const
{
    uint16_t newR(r + other.r), newG(g + other.g), newB(b + other.b);

    return {
        static_cast<uint8_t>(newR > UINT8_MAX ? UINT8_MAX : newR),
        static_cast<uint8_t>(newG > UINT8_MAX ? UINT8_MAX : newG),
        static_cast<uint8_t>(newB > UINT8_MAX ? UINT8_MAX : newB)
    };
}

TRgb TRgb::operator-(const TRgb& other) const
{
    int16_t newR(r - other.r), newG(g - other.g), newB(b - other.b);

    return {
        static_cast<uint8_t>(newR < 0 ? 0 : newR),
        static_cast<uint8_t>(newG < 0 ? 0 : newG),
        static_cast<uint8_t>(newB < 0 ? 0 : newB)
    };
}

TRgb& TRgb::operator+=(const TRgb& other)
{
    *this = *this + other;

    return *this;
}

TRgb& TRgb::operator-=(const TRgb& other)
{
    *this = *this - other;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const TRgb& color)
{
    return os << '#' << std::hex
        << std::setfill('0') << std::setw(2) << static_cast<int>(color.r)
        << std::setfill('0') << std::setw(2) << static_cast<int>(color.g)
        << std::setfill('0') << std::setw(2) << static_cast<int>(color.b);
}

/**
 * Construct color from float values, clamping if necessary.
 */
TRgb rgbFromFloat(float initialR, float initialG, float initialB)
{
    if(initialR > UINT8_MAX) {initialR = UINT8_MAX;}
    if(initialR <         0) {initialR =         0;}
    if(initialG > UINT8_MAX) {initialG = UINT8_MAX;}
    if(initialG <         0) {initialG =         0;}
    if(initialB > UINT8_MAX) {initialB = UINT8_MAX;}
    if(initialB <         0) {initialB =         0;}

    return TRgb((uint8_t)initialR, (uint8_t)initialG, (uint8_t)initialB);
}

bool TLinearConstants::operator==(const TLinearConstants &other) const
{
    return (factor == other.factor) && (offset == other.offset);
}

bool TLinearConstants::operator!=(const TLinearConstants &other) const
{
    return !(other == *this);
}

Json convert(const TNoteToLightMap& source)
{
    Json::object converted;
    for(const auto& pair : source)
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
    for(unsigned int noteNumber = 0; noteNumber <= UINT8_MAX; ++noteNumber)
    {
        char buf[4];
        snprintf(buf, sizeof(buf), "%u", noteNumber);

        uint8_t lightNumber;
        if(helper.getItemIfPresent(std::string(buf), lightNumber))
        {
            converted[noteNumber] = lightNumber;
        }
    }

    return converted;
}

} /* namespace Processing */


