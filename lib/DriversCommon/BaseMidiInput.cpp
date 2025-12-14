#include "BaseMidiInput.hpp"

#include "Logging.hpp"

#define LOGGING_COMPONENT "BaseMidiInput"

BaseMidiInput::BaseMidiInput() {}

void BaseMidiInput::subscribe(Observer& observer)
{
    std::lock_guard<std::mutex> lock(observersMutex);
    observers.push_back(&observer);
}

void BaseMidiInput::unsubscribe(Observer& observer)
{
    std::lock_guard<std::mutex> lock(observersMutex);
    observers.remove(&observer);
}

void BaseMidiInput::notifyNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity,
                                     bool on) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    for (auto observer : observers)
    {
        observer->onNoteChange(channel, pitch, velocity, on);
    }
}

void BaseMidiInput::notifyControlChange(uint8_t channel, MidiInterface::ControllerNumber controller,
                                        uint8_t value) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    for (auto observer : observers)
    {
        observer->onControlChange(channel, controller, value);
    }
}

void BaseMidiInput::notifyProgramChange(uint8_t channel, uint8_t program) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    for (auto observer : observers)
    {
        observer->onProgramChange(channel, program);
    }
}

void BaseMidiInput::notifyChannelPressureChange(uint8_t channel, uint8_t value) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    for (auto observer : observers)
    {
        observer->onChannelPressureChange(channel, value);
    }
}

void BaseMidiInput::notifyPitchBendChange(uint8_t channel, uint16_t value) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    for (auto observer : observers)
    {
        observer->onPitchBendChange(channel, value);
    }
}

void BaseMidiInput::processMidiByte(uint8_t value)
{
#ifdef DIAG_LIGHT
    diag.totalBytes++;
#endif
    if (!buildingMessage && ((value & 0x80) == 0x80))
    {
        // Is a status byte. Start building new message
        currentMessage.clear();
        buildingMessage = true;
    }

    if (buildingMessage)
    {
        currentMessage.push_back(value);
#ifdef DIAG_LIGHT
        if (currentMessage.size() > diag.maxMessageSize)
        {
            diag.maxMessageSize = static_cast<uint8_t>(currentMessage.size());
        }
#endif

        // Get status (high nibble) and channel (low nibble) from status byte
        uint8_t statusByte(currentMessage[0]);
        uint8_t status(statusByte & 0xF0);
        uint8_t channel(statusByte & 0x0F);

        // Check if a message can be parsed and sent to subscribers.
        switch (static_cast<MidiInterface::Status>(status))
        {
            case noteOff:
                if (currentMessage.size() >= 3)
                {
                    // Channel, pitch, velocity, note off
                    notifyNoteChange(channel, currentMessage[1], currentMessage[2], false);
                    buildingMessage = false;
#ifdef DIAG_LIGHT
                    diag.totalMessages++;
#endif
                }
                break;

            case noteOn:
                if (currentMessage.size() >= 3)
                {
                    // Channel, pitch, velocity, note on
                    notifyNoteChange(channel, currentMessage[1], currentMessage[2], true);
                    buildingMessage = false;
#ifdef DIAG_LIGHT
                    diag.totalMessages++;
#endif
                }
                break;

            case controlChange:
                if (currentMessage.size() >= 3)
                {
                    // Channel, controller number, value
                    notifyControlChange(channel, (MidiInterface::ControllerNumber)currentMessage[1],
                                        currentMessage[2]);
                    buildingMessage = false;
#ifdef DIAG_LIGHT
                    diag.totalMessages++;
#endif
                }
                break;

            case programChange:
                if (currentMessage.size() >= 2)
                {
                    // Channel, number
                    notifyProgramChange(channel, currentMessage[1]);
                    buildingMessage = false;
#ifdef DIAG_LIGHT
                    diag.totalMessages++;
#endif
                }
                break;

            case channelPressureChange:
                if (currentMessage.size() >= 2)
                {
                    // Channel, value
                    notifyChannelPressureChange(channel, currentMessage[1]);
                    buildingMessage = false;
#ifdef DIAG_LIGHT
                    diag.totalMessages++;
#endif
                }
                break;

            case pitchBendChange:
                if (currentMessage.size() >= 3)
                {
                    // Pitch bend value is a 14-bit value.
                    // The first byte contains the low 7 bits, the second byte the high 7 bits.
                    uint16_t value(currentMessage[1] | (currentMessage[2] << 7));

                    // Channel, value
                    notifyPitchBendChange(channel, value);
                    buildingMessage = false;
#ifdef DIAG_LIGHT
                    diag.totalMessages++;
#endif
                }
                break;

            default:
                // Unsupported status.
                LOG_WARNING_PARAMS(
                    "Unsupported MIDI status %#02x on channel %2u, ignoring rest of message.",
                    status, channel);
                buildingMessage = false;
#ifdef DIAG_LIGHT
                diag.unsupportedStatus++;
#endif
                break;
        }

#ifdef DIAG_LIGHT
        // If the message grows unusually without parsing, count as parse error and reset.
        // Typical MIDI channel messages are <=3 bytes; sysex is unsupported here.
        if (currentMessage.size() > 3)
        {
            // If we didn't recognize a status by now, treat as parse anomaly.
            // Do not spam logs: only increment a counter.
            diag.parseErrors++;
            currentMessage.clear();
            buildingMessage = false;
        }
#endif
    }
}
