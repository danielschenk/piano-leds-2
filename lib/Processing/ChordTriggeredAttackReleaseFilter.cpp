#include "ChordTriggeredAttackReleaseFilter.hpp"

void ChordTriggeredAttackReleaseFilter::execute(processing::RgbStrip& strip,
                                                const processing::NoteToLightMap& noteToLightMap)
{
}

ProcessingBlock::Mode ChordTriggeredAttackReleaseFilter::mode() const
{
    return Mode::overwriting;
}
