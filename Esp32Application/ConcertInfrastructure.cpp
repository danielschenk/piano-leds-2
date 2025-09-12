#include "ConcertInfrastructure.hpp"

#include "Color.hpp"
#include "HorizontalStretcher.hpp"
#include "LinearRgbFunction.hpp"
#include "Logging.hpp"
#include "NoteVisualizer.hpp"
#include "Patch.hpp"
#include "PianoDecayRgbFunction.hpp"
#include "SequentialColorPicker.hpp"
#include "SingleColorFill.hpp"

namespace application
{

#define LOGGING_COMPONENT "ConcertInfrastructure"

ConcertInfrastructure::ConcertInfrastructure(IMidiInput& midiInput, const ITime& time)
    : midiInput(midiInput),
      time(time),
      processingBlockFactory(midiInput, rgbFunctionFactory, time),
      concert(midiInput, processingBlockFactory)
{
    concert.setNoteToLightMap(createDefaultOneToOneFullPianoMapping(1));
    LOG_INFO_PARAMS("strip size: %zd", concert.getStripSize());
    createLegacyPatches();
}

void ConcertInfrastructure::createLegacyPatches()
{
    using namespace processing::color_constants;
    constexpr std::array<processing::RgbColor, 7> colors{red,     green, blue, yellow,
                                                         magenta, cyan,  white};

    uint8_t program = 0;
    for (const auto& color : colors)
    {
        auto patch = addBasicPatch(color, false);
        patch->setProgram(program++);
    }
    for (const auto& color : colors)
    {
        auto patch = addBasicPatch(color, true);
        patch->setProgram(program++);
    }

    auto patch = addBasicPatch(white, false);
    patch->setProgram(52);
    patch->setName("Peter Gunn Intro");
    auto fill = new SingleColorFill;
    fill->setColor(blue * 0.1f);
    patch->getProcessingChain().insertBlock(fill, 0);

    patch = addBasicPatch(white, true, std::make_shared<processing::SequentialColorPicker>());
    patch->setProgram(53);
    patch->setName("Multicolor");
}

Patch* ConcertInfrastructure::addBasicPatch(
    const processing::RgbColor& color, bool likePiano,
    std::shared_ptr<processing::ColorPicker> pressDownColorPicker)
{
    auto patch = new Patch(processingBlockFactory);
    auto block = new NoteVisualizer(midiInput, rgbFunctionFactory, time);
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
    block->setPressDownColorPicker(pressDownColorPicker);
    patch->getProcessingChain().insertBlock(block);

    // use those skipped leds for double brightness per note
    auto stretcher = new processing::HorizontalStretcher;
    patch->getProcessingChain().insertBlock(stretcher);

    concert.addPatch(patch);

    return patch;
}

processing::TNoteToLightMap ConcertInfrastructure::createDefaultOneToOneFullPianoMapping(
    uint16_t skipLedsPerNote)
{
    processing::TNoteToLightMap noteToLightMap;
    uint16_t lightNumber = 0;
    constexpr uint8_t noteNumberA0 = 21;
    constexpr uint8_t noteNumberC8 = 108;
    for (uint8_t noteNumber = noteNumberC8; noteNumber >= noteNumberA0; --noteNumber)
    {
        noteToLightMap[noteNumber] = lightNumber;
        lightNumber += (1 + skipLedsPerNote);
    }

    return noteToLightMap;
}

}  // namespace application
