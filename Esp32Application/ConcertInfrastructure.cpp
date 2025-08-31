#include "ConcertInfrastructure.h"
#include "Patch.h"
#include "NoteRgbSource.h"
#include "LinearRgbFunction.h"
#include "PianoDecayRgbFunction.h"

namespace application
{

ConcertInfrastructure::ConcertInfrastructure(IMidiInput& midiInput, const ITime& time)
    : midiInput(midiInput)
    , time(time)
    , processingBlockFactory(midiInput, rgbFunctionFactory, time)
    , concert(midiInput, processingBlockFactory)
{
    uint8_t lightNumber = 0;
    for(uint8_t noteNumber = 48 /* C below middle C */; noteNumber < 72; ++noteNumber)
    {
        noteToLightMap[noteNumber] = lightNumber;
        ++lightNumber;
    }
    concert.setNoteToLightMap(noteToLightMap);

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

}
