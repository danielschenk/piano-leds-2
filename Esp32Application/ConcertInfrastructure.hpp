#ifndef CONCERT_INFRASTRUCTURE_H
#define CONCERT_INFRASTRUCTURE_H

#include "Concert.hpp"
#include "ProcessingBlockFactory.hpp"
#include "ProcessingTypes.hpp"
#include "RgbFunctionFactory.hpp"

class MidiInput;
class MonotonicTime;
class Patch;

namespace processing
{
class ColorPicker;
}

namespace application
{

struct ConcertInfrastructure
{
    ConcertInfrastructure(MidiInput& midiInput, const MonotonicTime& time);
    void createLegacyPatches();
    Patch* addBasicPatch(const processing::RgbColor& color, bool likePiano,
                         std::shared_ptr<processing::ColorPicker> pressDownColorPicker = nullptr);
    static processing::NoteToLightMap createDefaultOneToOneFullPianoMapping(
        uint16_t skipLedsPerNote = 0);

    MidiInput& midiInput;
    const MonotonicTime& time;

    processing::RgbFunctionFactory rgbFunctionFactory;
    ProcessingBlockFactory processingBlockFactory;
    Concert concert;
};

}  // namespace application

#endif
