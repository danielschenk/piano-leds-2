#include "ConcertInfrastructure.hpp"

#include "ChordTriggeredEnvelopeFilter.hpp"
#include "Color.hpp"
#include "HorizontalStretcher.hpp"
#include "LinearRgbFunction.hpp"
#include "Logging.hpp"
#include "NoteVisualizer.hpp"
#include "OnOffRgbFunction.hpp"
#include "Patch.hpp"
#include "PianoDecayRgbFunction.hpp"
#include "SequentialColorPicker.hpp"
#include "SingleColorFill.hpp"
#include "Twinkles.hpp"

namespace application
{

#define LOGGING_COMPONENT "ConcertInfrastructure"

ConcertInfrastructure::ConcertInfrastructure(MidiInput& midiInput, const MonotonicTime& time)
    : midiInput(midiInput),
      time(time),
      processingBlockFactory(midiInput, rgbFunctionFactory, time),
      concert(midiInput, processingBlockFactory)
{
    concert.setNoteToLightMap(createDefaultOneToOneFullPianoMapping(1));
    concert.setStripSize(176);
    LOG_INFO_PARAMS("strip size: %zd", concert.getStripSize());
    createLegacyPatches();
    concert.reactivateLastActivePatch();
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
    auto fill = new SingleColorFill{blue * 0.1f};
    patch->getProcessingChain().insertBlock(fill, 0);

    patch = addBasicPatch(white, true, std::make_shared<processing::SequentialColorPicker>());
    patch->getProcessingChain().insertBlock(new SingleColorFill{white * 0.2f}, 0);
    // flash white every time first chord of cycle is played
    // only during intro (distinguished by added bass note)
    // 4s should be enough to not retrigger until in next cycle
    auto backgroundFlashFilter = new ChordTriggeredEnvelopeFilter{midiInput, time};
    backgroundFlashFilter->envelope = {0, 4000};
    backgroundFlashFilter->canRestart = false;
    backgroundFlashFilter->setNotes({43, 62, 67, 71});
    patch->getProcessingChain().insertBlock(backgroundFlashFilter, 1);
    patch->setProgram(53);
    patch->setName("Multicolor");

    auto xmasColorPicker = std::make_shared<processing::SequentialColorPicker>();
    xmasColorPicker->sequence = {red, green};
    patch = addBasicPatch(white, false, xmasColorPicker);
    patch->setProgram(54);
    patch->setName("Merry Xmas Everybody");
    patch->getProcessingChain().insertBlock(new processing::Twinkles(time));

    auto rastaColorPicker = std::make_shared<processing::SequentialColorPicker>();
    rastaColorPicker->sequence = {red, yellow, green};
    patch = addBasicPatch(white, false, rastaColorPicker);
    patch->setProgram(55);
    patch->setName("Don't Look Back");
}

Patch* ConcertInfrastructure::addBasicPatch(
    const processing::RgbColor& color, bool likePiano,
    std::shared_ptr<processing::ColorPicker> pressDownColorPicker)
{
    auto patch = new Patch(processingBlockFactory);
    auto block = new NoteVisualizer(midiInput, time);
    std::shared_ptr<processing::RgbFunction> rgbFunction;
    if (likePiano)
        rgbFunction = std::make_shared<processing::PianoDecayRgbFunction>(color);
    else
        rgbFunction = std::make_shared<processing::OnOffRgbFunction>(color);
    block->setUsingPedal(true);
    block->setRgbFunction(rgbFunction);
    block->setPressDownColorPicker(pressDownColorPicker);
    patch->getProcessingChain().insertBlock(block);

    // use those skipped leds for double brightness per note
    auto stretcher = new processing::HorizontalStretcher;
    patch->getProcessingChain().insertBlock(stretcher);

    concert.addPatch(patch);

    return patch;
}

processing::NoteToLightMap ConcertInfrastructure::createDefaultOneToOneFullPianoMapping(
    uint16_t skipLedsPerNote)
{
    processing::NoteToLightMap noteToLightMap;
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
