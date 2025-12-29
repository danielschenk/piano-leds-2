#include "NoteVisualizer.hpp"

#include <functional>

#include "Logging.hpp"
#include "RgbFunction.hpp"

#define LOGGING_COMPONENT "NoteVisualizer"

void NoteVisualizer::execute(processing::RgbStrip& strip, const Input& input)
{
    if (!rgbFunction)
        return;

    for (const auto& [note, lightIndex] : input.noteToLightMap)
    {
        if (lightIndex < strip.size())
            strip[lightIndex] =
                rgbFunction->calculate(input.noteStates[midiSlot].get()[note], input.nowMs);
    }
}

bool NoteVisualizer::valid(const Input& input)
{
    return midiSlot < input.noteStates.size();
}

std::string NoteVisualizer::getObjectType() const
{
    return ProcessingBlock::typeNameNoteVisualizer;
}
