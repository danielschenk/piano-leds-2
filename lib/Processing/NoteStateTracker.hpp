#ifndef PROCESSING_NOTESTATETRACKER_HPP
#define PROCESSING_NOTESTATETRACKER_HPP

#include <array>
#include <memory>
#include <mutex>

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
    const processing::NoteStates& noteStates() const;

    uint8_t channel{0};

  private:
    // MidiInput::Observer implementation
    void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) override;
    void onControlChange(uint8_t channel, MidiInput::ControllerNumber controller,
                         uint8_t value) override;

    mutable std::mutex mutex;

    MidiInput& midiInput;
    Scheduler scheduler;
    processing::NoteStates noteStates_;
    bool pedalPressed{false};
    std::shared_ptr<processing::ColorPicker> pressDownColorPicker;
    const MonotonicTime& time;
};

#endif /* PROCESSING_NOTESTATETRACKER_HPP */
