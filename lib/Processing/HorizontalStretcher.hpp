#ifndef PROCESSING_DUPLICATOR
#define PROCESSING_DUPLICATOR

#include "ProcessingBlock.hpp"

namespace processing
{

class HorizontalStretcher : public ProcessingBlock
{
  public:
    HorizontalStretcher() = default;
    void execute(RgbStrip& strip, const Input& input) override;
    Mode mode() const override;
};

}  // namespace processing

#endif
