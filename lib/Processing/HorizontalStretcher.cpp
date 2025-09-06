#include "HorizontalStretcher.hpp"

namespace Processing
{

void HorizontalStretcher::execute(TRgbStrip& strip, const TNoteToLightMap& noteToLightMap)
{
    for (size_t ledIdx = 0; (ledIdx + 1) < strip.size(); ledIdx += 2)
        strip[ledIdx+1] = strip[ledIdx];
}

HorizontalStretcher::Mode HorizontalStretcher::mode() const
{
    return Mode::overwriting;
}

}
