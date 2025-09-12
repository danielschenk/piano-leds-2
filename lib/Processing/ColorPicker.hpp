#ifndef PROCESSING_COLORPICKER_HPP
#define PROCESSING_COLORPICKER_HPP

#include "ProcessingTypes.hpp"

namespace Processing
{

class ColorPicker
{
  public:
    virtual TRgb pick() = 0;
};

}  // namespace Processing

#endif
