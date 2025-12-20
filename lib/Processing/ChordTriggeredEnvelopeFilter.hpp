#ifndef PROCESSING_CHORDTIRGGEREDATTACKRELEASEFILTER_HPP
#define PROCESSING_CHORDTIRGGEREDATTACKRELEASEFILTER_HPP

#include <cstdint>
#include <map>
#include <set>

#include "MidiInput.hpp"
#include "ProcessingBlock.hpp"
#include "Scheduler.hpp"
#include "TimeEnvelope.hpp"

class MonotonicTime;

class ChordTriggeredEnvelopeFilter : public ProcessingBlock, MidiInput::Observer
{
  public:
    ChordTriggeredEnvelopeFilter(MidiInput& midiInput, const MonotonicTime& time);

    processing::TimeEnvelope envelope{0, 4000};
    bool canRestart{true};

    uint8_t channel{0};

    void setNotes(const std::set<uint8_t>& notes);

    // ProcessingBlock
    void activate() override;
    void deactivate() override;
    void execute(processing::RgbStrip& strip,
                 const processing::NoteToLightMap& noteToLightMap) override;
    Mode mode() const override;

  private:
    // MidiInput::Observer
    void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) override;

    void calculateTrigger(uint32_t now);
    void render(processing::RgbStrip& strip, uint32_t now);

    std::map<uint8_t, bool> chordNotes;

    MidiInput& midiInput;
    const MonotonicTime& time;
    std::optional<uint32_t> triggerTimeMs;
    Scheduler scheduler;
};

#endif
