#include "Color.hpp"

#include <iomanip>

namespace processing
{

RgbColor& RgbColor::operator*=(const RgbColor& other)
{
    *this = *this * other;
    return *this;
}

RgbColor& RgbColor::operator+=(const RgbColor& other)
{
    *this = *this + other;
    return *this;
}

RgbColor& RgbColor::operator-=(const RgbColor& other)
{
    *this = *this - other;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const RgbColor& color)
{
    return os << '#' << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(color.r)
              << std::setfill('0') << std::setw(2) << static_cast<int>(color.g) << std::setfill('0')
              << std::setw(2) << static_cast<int>(color.b);
}

}  // namespace processing
