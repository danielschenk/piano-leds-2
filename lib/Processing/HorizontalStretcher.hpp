#ifndef PROCESSING_DUPLICATOR
#define PROCESSING_DUPLICATOR

#include "ProcessingBlock.hpp"

namespace Processing
{

class HorizontalStretcher : public ProcessingBlock
{
  public:
    HorizontalStretcher() = default;
    void execute(TRgbStrip& strip, const TNoteToLightMap& noteToLightMap) override;
    Mode mode() const override;
};

}  // namespace Processing

#endif
