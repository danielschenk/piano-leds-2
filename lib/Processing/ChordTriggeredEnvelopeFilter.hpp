#ifndef PROCESSING_CHORDTRIGGEREDENVELOPEFILTER_HPP
#define PROCESSING_CHORDTRIGGEREDENVELOPEFILTER_HPP

#include <cstdint>
#include <map>
#include <optional>
#include <set>

#include "MidiInput.hpp"
#include "ProcessingBlock.hpp"
#include "Scheduler.hpp"
#include "TimeEnvelope.hpp"

class ChordTriggeredEnvelopeFilter : public ProcessingBlock, MidiInput::Observer
{
  public:
    explicit ChordTriggeredEnvelopeFilter(MidiInput& midiInput);

    processing::TimeEnvelope envelope;
    bool canRestart{true};

    uint8_t channel{0};

    void setNotes(const std::set<uint8_t>& notes);

    // ProcessingBlock
    void activate() override;
    void deactivate() override;
    void execute(processing::RgbStrip& strip, const ProcessingBlock::Input& input) override;
    Mode mode() const override;

  private:
    // MidiInput::Observer
    void onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity, bool on) override;

    void calculateTrigger(uint32_t now);
    void render(processing::RgbStrip& strip, uint32_t now);

    std::map<uint8_t, bool> chordNotes;

    MidiInput& midiInput;
    std::optional<uint32_t> triggerTimeMs;
    Scheduler scheduler;
};

#endif
