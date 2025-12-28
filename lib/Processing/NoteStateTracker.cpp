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

const processing::NoteStates& NoteStateTracker::noteStates() const
{
    return noteStates_;
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
                    noteStates_[number].pressDownVelocity = velocity;
                    noteStates_[number].noteOnTimeStamp = time.getMilliseconds();
                    noteStates_[number].pressed = true;
                    noteStates_[number].sounding = true;
                    if (pressDownColorPicker)
                        noteStates_[number].pressDownColor = pressDownColorPicker->pick();
                }
                else
                {
                    noteStates_[number].pressed = false;
                    if (!pedalPressed)
                    {
                        noteStates_[number].sounding = false;
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

            if (channel == this->channel)
            {
                pedalPressed = (value >= 64);
                if (!pedalPressed)
                {
                    // Stop all notes which are sounding due to pedal only
                    for (int note = 0; note < MidiInterface::numNotes; ++note)
                    {
                        if (!noteStates_[note].pressed)
                        {
                            noteStates_[note].sounding = false;
                        }
                    }
                }
            }
        });
}
