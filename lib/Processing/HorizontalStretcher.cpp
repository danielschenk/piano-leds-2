#include "HorizontalStretcher.hpp"

namespace processing
{

void HorizontalStretcher::execute(RgbStrip& strip, const NoteToLightMap& noteToLightMap)
{
    for (size_t ledIdx = 0; (ledIdx + 1) < strip.size(); ledIdx += 2)
        strip[ledIdx + 1] = strip[ledIdx];
}

HorizontalStretcher::Mode HorizontalStretcher::mode() const
{
    return Mode::overwriting;
}

}  // namespace processing
