#ifndef CONCERT_INFRASTRUCTURE_H
#define CONCERT_INFRASTRUCTURE_H

#include "Concert.h"
#include "ProcessingBlockFactory.h"
#include "RgbFunctionFactory.h"
#include "ProcessingTypes.h"

class IMidiInput;
class ITime;
class Patch;

namespace Processing
{
class ColorPicker;
}

namespace application
{

struct ConcertInfrastructure
{
    ConcertInfrastructure(IMidiInput& midiInput, const ITime& time);
    void createLegacyPatches();
    Patch* addBasicPatch(const Processing::TRgb& color, bool likePiano, std::shared_ptr<Processing::ColorPicker> pressDownColorPicker = nullptr);
    static Processing::TNoteToLightMap createDefaultOneToOneFullPianoMapping(uint16_t skipLedsPerNote = 0);

    IMidiInput& midiInput;
    const ITime& time;

    RgbFunctionFactory rgbFunctionFactory;
    ProcessingBlockFactory processingBlockFactory;
    Concert concert;
};

}

#endif
