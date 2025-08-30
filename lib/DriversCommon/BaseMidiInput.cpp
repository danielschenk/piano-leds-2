#include "BaseMidiInput.h"

#include <Logging.h>

#define LOGGING_COMPONENT "BaseMidiInput"

BaseMidiInput::BaseMidiInput()
    : observers()
    , buildingMessage(false)
    , currentMessage()
    , observersMutex()
{
}

void BaseMidiInput::subscribe(IObserver& observer)
{
    std::lock_guard<std::mutex> lock(observersMutex);
    observers.push_back(&observer);
}

void BaseMidiInput::unsubscribe(IObserver& observer)
{
    std::lock_guard<std::mutex> lock(observersMutex);
    observers.remove(&observer);
}

void BaseMidiInput::notifyNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    for(auto observer : observers)
    {
        observer->onNoteChange(channel, pitch, velocity, on);
    }
}

void BaseMidiInput::notifyControlChange(uint8_t channel, IMidiInterface::TControllerNumber controller, uint8_t value) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    for(auto observer : observers)
    {
        observer->onControlChange(channel, controller, value);
    }
}

void BaseMidiInput::notifyProgramChange(uint8_t channel, uint8_t program) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    for(auto observer : observers)
    {
        observer->onProgramChange(channel, program);
    }
}

void BaseMidiInput::notifyChannelPressureChange(uint8_t channel, uint8_t value) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    for(auto observer : observers)
    {
        observer->onChannelPressureChange(channel, value);
    }
}

void BaseMidiInput::notifyPitchBendChange(uint8_t channel, uint16_t value) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    for(auto observer : observers)
    {
        observer->onPitchBendChange(channel, value);
    }
}

void BaseMidiInput::processMidiByte(uint8_t value)
{
    if(!buildingMessage && ((value & 0x80) == 0x80))
    {
        // Is a status byte. Start building new message
        currentMessage.clear();
        buildingMessage = true;
    }

    if(buildingMessage)
    {
        currentMessage.push_back(value);

        // Get status (high nibble) and channel (low nibble) from status byte
        uint8_t statusByte(currentMessage[0]);
        uint8_t status(statusByte & 0xF0);
        uint8_t channel(statusByte & 0x0F);

        // Check if a message can be parsed and sent to subscribers.
        switch(static_cast<IMidiInterface::TStatus>(status))
        {
        case NOTE_OFF:
            if(currentMessage.size() >= 3)
            {
                // Channel, pitch, velocity, note off
                notifyNoteChange(channel, currentMessage[1], currentMessage[2], false);
                buildingMessage = false;
            }
            break;

        case NOTE_ON:
            if(currentMessage.size() >= 3)
            {
                // Channel, pitch, velocity, note on
                notifyNoteChange(channel, currentMessage[1], currentMessage[2], true);
                buildingMessage = false;
            }
            break;

        case CONTROL_CHANGE:
            if(currentMessage.size() >= 3)
            {
                // Channel, controller number, value
                notifyControlChange(channel, (IMidiInterface::TControllerNumber)currentMessage[1], currentMessage[2]);
                buildingMessage = false;
            }
            break;

        case PROGRACHANGE:
            if(currentMessage.size() >= 2)
            {
                // Channel, number
                notifyProgramChange(channel, currentMessage[1]);
                buildingMessage = false;
            }
            break;

        case CHANNEL_PRESSURE_CHANGE:
            if(currentMessage.size() >= 2)
            {
                // Channel, value
                notifyChannelPressureChange(channel, currentMessage[1]);
                buildingMessage = false;
            }
            break;

        case PITCH_BEND_CHANGE:
            if(currentMessage.size() >= 3)
            {
                // Pitch bend value is a 14-bit value.
                // The first byte contains the low 7 bits, the second byte the high 7 bits.
                uint16_t value(currentMessage[1] | (currentMessage[2] << 7));

                // Channel, value
                notifyPitchBendChange(channel, value);
                buildingMessage = false;
            }
            break;

        default:
            // Unsupported status.
            LOG_WARNING_PARAMS("Unsupported MIDI status %#02x on channel %2u, ignoring rest of message.", status, channel);
            buildingMessage = false;
            break;
        }
    }
}
