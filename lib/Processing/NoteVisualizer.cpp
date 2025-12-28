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
                rgbFunction->calculate(input.noteStates.at(channel).get()[note], input.nowMs);
    }
}

std::string NoteVisualizer::getObjectType() const
{
    return ProcessingBlock::typeNameNoteVisualizer;
}
