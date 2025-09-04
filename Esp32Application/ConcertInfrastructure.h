#ifndef CONCERT_INFRASTRUCTURE_H
#define CONCERT_INFRASTRUCTURE_H

#include "Concert.h"
#include "ProcessingBlockFactory.h"
#include "RgbFunctionFactory.h"
#include "ProcessingTypes.h"

class IMidiInput;
class ITime;

namespace application
{

struct ConcertInfrastructure
{
    ConcertInfrastructure(IMidiInput& midiInput, const ITime& time);
    void createLegacyPatches();
    static Processing::TNoteToLightMap createDefaultOneToOneFullPianoMapping(uint16_t skipLedsPerNote = 0);

    IMidiInput& midiInput;
    const ITime& time;

    RgbFunctionFactory rgbFunctionFactory;
    ProcessingBlockFactory processingBlockFactory;
    Concert concert;
};

}

#endif
