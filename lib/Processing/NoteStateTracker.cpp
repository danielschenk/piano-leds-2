#include "NoteStateTracker.hpp"

#include "MonotonicTime.hpp"

NoteStateTracker::NoteStateTracker(MidiInput& midiInput, const MonotonicTime& time)
    : midiInput(midiInput), time(time)
{
    midiInput.subscribe(*this);
}

NoteStateTracker::~NoteStateTracker()
{
    midiInput.unsubscribe(*this);
}

void NoteStateTracker::execute()
{
    scheduler.executeAll();
}

void NoteStateTracker::onNoteChange(uint8_t channel, uint8_t number, uint8_t velocity, bool on)
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

void NoteStateTracker::onControlChange(uint8_t channel, MidiInput::ControllerNumber number,
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

const std::vector<processing::NoteState>& NoteStateTracker::getNoteStates() const
{
    return noteStates;
}

uint8_t NoteStateTracker::getChannel() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return channel;
}

void NoteStateTracker::setChannel(uint8_t channel)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->channel = channel;
}

bool NoteStateTracker::isUsingPedal() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return usingPedal;
}

void NoteStateTracker::setUsingPedal(bool usingPedal)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->usingPedal = usingPedal;
}
