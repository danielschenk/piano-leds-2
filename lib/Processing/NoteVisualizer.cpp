#include "NoteVisualizer.hpp"

#include <functional>

#include "ColorPicker.hpp"
#include "Logging.hpp"
#include "MidiInput.hpp"
#include "MonotonicTime.hpp"
#include "NoteStateTracker.hpp"
#include "RgbFunction.hpp"

#define LOGGING_COMPONENT "NoteVisualizer"

void NoteVisualizer::execute(processing::RgbStrip& strip, const Input& input)
{
    for (auto pair : input.noteToLightMap)
    {
        // first: note number, second: light number
        if (rgbFunction != nullptr && pair.second < strip.size())
            strip[pair.second] = rgbFunction->calculate(input.noteStates[pair.first], input.nowMs);
    }
}

void NoteVisualizer::setRgbFunction(std::shared_ptr<processing::RgbFunction> rgbFunction)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->rgbFunction = rgbFunction;
}

std::string NoteVisualizer::getObjectType() const
{
    return ProcessingBlock::typeNameNoteVisualizer;
}
