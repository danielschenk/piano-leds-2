#include "NoteVisualizer.hpp"

#include <functional>

#include "ColorPicker.hpp"
#include "IRgbFunctionFactory.hpp"
#include "Json11Helper.hpp"
#include "Logging.hpp"
#include "MonotonicTime.hpp"
#include "RgbFunction.hpp"

#define LOGGING_COMPONENT "NoteVisualizer"

NoteVisualizer::NoteVisualizer(MidiInput& midiInput,
                               const processing::IRgbFunctionFactory& rgbFunctionFactory,
                               const MonotonicTime& time)
    : rgbFunctionFactory(rgbFunctionFactory), midiInput(midiInput), time(time)
{
}

void NoteVisualizer::activate()
{
    midiInput.subscribe(*this);

    std::lock_guard<std::mutex> lock(mutex);
    noteStates.resize(MidiInterface::numNotes);
}

void NoteVisualizer::deactivate()
{
    midiInput.unsubscribe(*this);
    scheduler.clear();

    std::lock_guard<std::mutex> lock(mutex);
    noteStates.resize(0);
    noteStates.shrink_to_fit();
}

void NoteVisualizer::execute(processing::RgbStrip& strip,
                             const processing::NoteToLightMap& noteToLightMap)
{
    scheduler.executeAll();

    for (auto pair : noteToLightMap)
    {
        // first: note number, second: light number
        if (rgbFunction != nullptr && pair.second < strip.size())
        {
            strip[pair.second] =
                rgbFunction->calculate(noteStates[pair.first], time.getMilliseconds());
        }
    }
}

void NoteVisualizer::onNoteChange(uint8_t channel, uint8_t number, uint8_t velocity, bool on)
{
    scheduler.schedule(
        [this, channel, number, velocity, on]()
        {
            std::lock_guard<std::mutex> lock(mutex);

            if (channel == this->channel)
            {
                if (on)
                {
                    noteStates[number].pressDownVelocity = velocity;
                    noteStates[number].noteOnTimeStamp = time.getMilliseconds();
                    noteStates[number].pressed = true;
                    noteStates[number].sounding = true;
                    if (pressDownColorPicker)
                        noteStates[number].pressDownColor = pressDownColorPicker->pick();
                }
                else
                {
                    noteStates[number].pressed = false;
                    if (!pedalPressed)
                    {
                        noteStates[number].sounding = false;
                    }
                }
            }
        });
}

void NoteVisualizer::onControlChange(uint8_t channel, MidiInput::ControllerNumber number,
                                     uint8_t value)
{
    if (number != MidiInterface::damperPedal)
        return;

    scheduler.schedule(
        [this, channel, value]()
        {
            std::lock_guard<std::mutex> lock(mutex);

            if (channel == this->channel && usingPedal)
            {
                pedalPressed = (value >= 64);
                if (!pedalPressed)
                {
                    // Stop all notes which are sounding due to pedal only
                    for (int note = 0; note < MidiInterface::numNotes; ++note)
                    {
                        if (!noteStates[note].pressed)
                        {
                            noteStates[note].sounding = false;
                        }
                    }
                }
            }
        });
}

uint8_t NoteVisualizer::getChannel() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return channel;
}

void NoteVisualizer::setChannel(uint8_t channel)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->channel = channel;
}

bool NoteVisualizer::isUsingPedal() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return usingPedal;
}

void NoteVisualizer::setUsingPedal(bool usingPedal)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->usingPedal = usingPedal;
}

void NoteVisualizer::setRgbFunction(std::shared_ptr<processing::RgbFunction> rgbFunction)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->rgbFunction = rgbFunction;
}

void NoteVisualizer::setPressDownColorPicker(std::shared_ptr<processing::ColorPicker> colorPicker)
{
    std::lock_guard<std::mutex> lock(mutex);
    pressDownColorPicker = colorPicker;
}

Json NoteVisualizer::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object json;
    json[JsonConvertible::objectTypeKey] = getObjectType();
    json[usingPedalJsonKey] = usingPedal;
    json[channelJsonKey] = channel;
    if (rgbFunction != nullptr)
    {
        json[rgbFunctionJsonKey] = rgbFunction->convertToJson();
    }

    return json;
}

void NoteVisualizer::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);

    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(usingPedalJsonKey, usingPedal);
    helper.getItemIfPresent(channelJsonKey, channel);

    Json::object convertedRgbFunction;
    if (helper.getItemIfPresent(rgbFunctionJsonKey, convertedRgbFunction))
        rgbFunction = rgbFunctionFactory.createRgbFunction(convertedRgbFunction);
}

std::string NoteVisualizer::getObjectType() const
{
    return ProcessingBlock::typeNameNoteVisualizer;
}
