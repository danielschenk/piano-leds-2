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

    IMidiInput& midiInput;
    const ITime& time;

    Processing::TNoteToLightMap noteToLightMap;
    RgbFunctionFactory rgbFunctionFactory;
    ProcessingBlockFactory processingBlockFactory;
    Concert concert;
};

}

#endif
