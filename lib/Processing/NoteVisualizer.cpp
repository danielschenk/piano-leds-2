#include "NoteVisualizer.h"

#include "IRgbFunctionFactory.h"
#include "IRgbFunction.h"
#include "ITime.h"
#include "Json11Helper.h"
#include "Logging.h"

#include <functional>

#define LOGGING_COMPONENT "NoteVisualizer"

NoteVisualizer::NoteVisualizer(IMidiInput& midiInput,
                             const IRgbFunctionFactory& rgbFunctionFactory,
                             const ITime& time)
    : rgbFunctionFactory(rgbFunctionFactory)
    , midiInput(midiInput)
    , time(time)
{
    midiInput.subscribe(*this);
}

NoteVisualizer::~NoteVisualizer()
{
    midiInput.unsubscribe(*this);
}

void NoteVisualizer::activate()
{
    std::lock_guard<std::mutex> lock(mutex);

    active = true;
}

void NoteVisualizer::deactivate()
{
    std::lock_guard<std::mutex> lock(mutex);

    // Make sure no notes stay active. Handle remaining events first.
    scheduler.executeAll();
    for(auto& noteState : noteStates)
    {
        noteState.pressed = false;
        noteState.sounding = false;
    }

    active = false;
}

void NoteVisualizer::execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap)
{
    scheduler.executeAll();

    for(auto pair : noteToLightMap)
    {
        // first: note number, second: light number
        if(rgbFunction != nullptr && pair.second < strip.size())
        {
            strip[pair.second] = rgbFunction->calculate(noteStates[pair.first], time.getMilliseconds());
        }
    }
}

void NoteVisualizer::onNoteChange(uint8_t channel, uint8_t number, uint8_t velocity, bool on)
{
    std::lock_guard<std::mutex> lock(mutex);

    if(!active)
    {
        return;
    }

    scheduler.schedule([this, channel, number, velocity, on]() {
        std::lock_guard<std::mutex> lock(mutex);

        if(channel == this->channel)
        {
            if(on)
            {
                noteStates[number].pressDownVelocity = velocity;
                noteStates[number].noteOnTimeStamp = time.getMilliseconds();
                noteStates[number].pressed = true;
                noteStates[number].sounding = true;
            }
            else
            {
                noteStates[number].pressed = false;
                if(!pedalPressed)
                {
                    noteStates[number].sounding = false;
                }
            }
        }
    });
}

void NoteVisualizer::onControlChange(uint8_t channel, IMidiInput::TControllerNumber number, uint8_t value)
{
    std::lock_guard<std::mutex> lock(mutex);

    if(!active)
    {
        return;
    }

    // Don't cause scheduling overhead when it's an unimportant controller number.
    // Channel check must be scheduled as it uses a member
    if(number == IMidiInterface::DAMPER_PEDAL)
    {
        scheduler.schedule([this, channel, value]() {
            std::lock_guard<std::mutex> lock(mutex);

            if(channel == this->channel && usingPedal)
            {
                pedalPressed = (value >= 64);
                if(!pedalPressed)
                {
                    // Stop all notes which are sounding due to pedal only
                    for(int note = 0; note < IMidiInterface::numNotes; ++note)
                    {
                        if(!noteStates[note].pressed)
                        {
                            noteStates[note].sounding = false;
                        }
                    }
                }
            }
        });
    }
}

void NoteVisualizer::onProgramChange(uint8_t channel, uint8_t program)
{
    // ignore
}

void NoteVisualizer::onChannelPressureChange(uint8_t channel, uint8_t value)
{
    // ignore
}

void NoteVisualizer::onPitchBendChange(uint8_t channel, uint16_t value)
{
    // ignore
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

void NoteVisualizer::setRgbFunction(std::shared_ptr<IRgbFunction> rgbFunction)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->rgbFunction = rgbFunction;
}

Json NoteVisualizer::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object json;
    json[JsonConvertible::objectTypeKey] = getObjectType();
    json[usingPedalJsonKey] = usingPedal;
    json[channelJsonKey] = channel;
    if(rgbFunction != nullptr)
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
    if(helper.getItemIfPresent(rgbFunctionJsonKey, convertedRgbFunction))
        rgbFunction = rgbFunctionFactory.createRgbFunction(convertedRgbFunction);
}

std::string NoteVisualizer::getObjectType() const
{
    return ProcessingBlock::typeNameNoteVisualizer;
}
