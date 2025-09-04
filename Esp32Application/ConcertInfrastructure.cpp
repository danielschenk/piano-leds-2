#include "ConcertInfrastructure.h"
#include "Patch.h"
#include "NoteRgbSource.h"
#include "LinearRgbFunction.h"
#include "PianoDecayRgbFunction.h"
#include "ProcessingTypes.h"
#include "Logging.h"

namespace application
{

#define LOGGING_COMPONENT "ConcertInfrastructure"

ConcertInfrastructure::ConcertInfrastructure(IMidiInput& midiInput, const ITime& time)
    : midiInput(midiInput)
    , time(time)
    , processingBlockFactory(midiInput, rgbFunctionFactory, time)
    , concert(midiInput, processingBlockFactory)
{
    concert.setNoteToLightMap(createDefaultOneToOneFullPianoMapping(1));
    LOG_INFO_PARAMS("strip size: %zd", concert.getStripSize());
    createLegacyPatches();
}

void ConcertInfrastructure::createLegacyPatches()
{
    using namespace Processing::ColorValue;
    constexpr std::array<Processing::TRgb, 7> colors{red, green, blue, yellow, magenta,
        cyan, white};

    uint8_t program = 0, bank = 0;
    for (int i = 0; i < 2; ++i)
    {
        for (const auto& color : colors)
        {
            auto patch = new Patch(processingBlockFactory);
            patch->setBank(bank);
            patch->setProgram(program++);

            auto block = new NoteRgbSource(midiInput, rgbFunctionFactory, time);
            bool likePiano(i > 0);
            std::shared_ptr<IRgbFunction> rgbFunction;
            if (likePiano)
            {
                rgbFunction = std::make_shared<PianoDecayRgbFunction>(color);
                block->setUsingPedal(true);
            }
            else
            {
                rgbFunction = std::make_shared<LinearRgbFunction>(color);
            }
            block->setRgbFunction(rgbFunction);
            patch->getProcessingChain().insertBlock(block);
            concert.addPatch(patch);
        }
    }
}

Processing::TNoteToLightMap ConcertInfrastructure::createDefaultOneToOneFullPianoMapping(uint16_t skipLedsPerNote)
{
    Processing::TNoteToLightMap noteToLightMap;
    uint16_t lightNumber = 0;
    constexpr uint8_t noteNumberA0 = 21;
    constexpr uint8_t noteNumberC8 = 108;
    for(uint8_t noteNumber = noteNumberA0; noteNumber <= noteNumberC8; ++noteNumber)
    {
        noteToLightMap[noteNumber] = lightNumber;
        lightNumber += (1 + skipLedsPerNote);
    }

    return noteToLightMap;
}

}
