#ifndef PROCESSING_NOTESTATETRACKER_HPP
#define PROCESSING_NOTESTATETRACKER_HPP

#include <memory>
#include <mutex>
#include <vector>

#include "ColorPicker.hpp"
#include "MidiInput.hpp"
#include "ProcessingTypes.hpp"
#include "Scheduler.hpp"

class MonotonicTime;

class NoteStateTracker : MidiInput::Observer
{
  public:
    NoteStateTracker(MidiInput& midiInput, const MonotonicTime& time);
    ~NoteStateTracker();

    NoteStateTracker(NoteStateTracker&) = delete;
    NoteStateTracker& operator=(NoteStateTracker&) = delete;

    void execute();

    uint8_t getChannel() const;
    void setChannel(uint8_t channel);
    bool isUsingPedal() const;
    void setUsingPedal(bool usingPedal);

  private:
    // MidiInput::Observer implementation
    void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) override;
    void onControlChange(uint8_t channel, MidiInput::ControllerNumber controller,
                         uint8_t value) override;

    mutable std::mutex mutex;

    bool usingPedal{true};
    MidiInput& midiInput;
    uint8_t channel = 0;
    Scheduler scheduler;
    std::vector<processing::NoteState> noteStates;
    bool pedalPressed{false};
    std::shared_ptr<processing::ColorPicker> pressDownColorPicker;
    const MonotonicTime& time;
};

#endif /* PROCESSING_NOTESTATETRACKER_HPP */
