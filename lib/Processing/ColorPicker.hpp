#ifndef PROCESSING_COLORPICKER_HPP
#define PROCESSING_COLORPICKER_HPP

#include "Color.hpp"

namespace processing
{

class ColorPicker
{
  public:
    virtual RgbColor pick() = 0;
};

}  // namespace processing

#endif
