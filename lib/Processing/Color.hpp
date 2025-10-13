#ifndef PROCESSING_COLOR_HPP
#define PROCESSING_COLOR_HPP

#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

namespace processing
{

namespace detail
{

template <class T>
constexpr T clamp(T v)
{
    return v < std::numeric_limits<uint8_t>::min()   ? std::numeric_limits<uint8_t>::min()
           : v > std::numeric_limits<uint8_t>::max() ? std::numeric_limits<uint8_t>::max()
                                                     : v;
}

}  // namespace detail

struct RgbColor
{
    constexpr RgbColor(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    constexpr RgbColor(float r, float g, float b)
        : RgbColor(static_cast<uint8_t>(detail::clamp(r)), static_cast<uint8_t>(detail::clamp(g)),
                   static_cast<uint8_t>(detail::clamp(b)))
    {
    }
    constexpr RgbColor(int r, int g, int b)
        : RgbColor(static_cast<uint8_t>(detail::clamp(r)), static_cast<uint8_t>(detail::clamp(g)),
                   static_cast<uint8_t>(detail::clamp(b)))
    {
    }
    constexpr RgbColor() = default;
    constexpr RgbColor(const RgbColor& other) = default;

    RgbColor& operator=(const RgbColor& other) = default;

    constexpr bool operator==(const RgbColor& other) const
    {
        return (other.r == r) && (other.g == g) && (other.b == b);
    }

    constexpr bool operator!=(const RgbColor& other) const
    {
        return !(other == *this);
    }

    // Multiplies every color using other color as factor (meaning 255=100%).
    constexpr RgbColor operator*(const RgbColor& other) const
    {
        return RgbColor(other.r / 255.0f * r, other.g / 255.0f * g, other.b / 255.0f * b);
    }

    constexpr RgbColor operator+(const RgbColor& other) const
    {
        return RgbColor(r + other.r, g + other.g, b + other.b);
    }

    constexpr RgbColor operator-(const RgbColor& other) const
    {
        return RgbColor(r - other.r, g - other.g, b - other.b);
    }

    // Multiplies every color using other color as factor (meaning 255=100%).
    RgbColor& operator*=(const RgbColor& other);

    RgbColor& operator+=(const RgbColor& other);
    RgbColor& operator-=(const RgbColor& other);

    // Implements custom value printing for Google Test
    friend std::ostream& operator<<(std::ostream& os, const RgbColor& color);

    uint8_t r{0};
    uint8_t g{0};
    uint8_t b{0};
};

constexpr RgbColor operator*(const RgbColor& other, float factor)
{
    return RgbColor(factor * other.r, factor * other.g, factor * other.b);
}

constexpr RgbColor operator*(float factor, const RgbColor& other)
{
    return other * factor;
}

namespace color_constants
{
constexpr RgbColor off{};
constexpr RgbColor red{0xff, 0x00, 0x00};
constexpr RgbColor green{0x00, 0xff, 0x00};
constexpr RgbColor blue{0x00, 0x00, 0xff};
constexpr RgbColor yellow{red + green};
constexpr RgbColor magenta{red + blue};
constexpr RgbColor cyan{green + blue};
constexpr RgbColor white{red + green + blue};
}  // namespace color_constants

typedef std::vector<RgbColor> RgbStrip;

}  // namespace processing

#endif
