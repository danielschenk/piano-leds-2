#include "ChordTriggeredAttackReleaseFilter.hpp"

void ChordTriggeredAttackReleaseFilter::execute(processing::RgbStrip& strip, const Input& input) {}

ProcessingBlock::Mode ChordTriggeredAttackReleaseFilter::mode() const
{
    return Mode::overwriting;
}
