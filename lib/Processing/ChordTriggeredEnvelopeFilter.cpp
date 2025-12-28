#include "ChordTriggeredEnvelopeFilter.hpp"

#include <algorithm>

#include "Logging.hpp"

#define LOGGING_COMPONENT "ChordTriggeredEnvelopeFilter"

ChordTriggeredEnvelopeFilter::ChordTriggeredEnvelopeFilter(MidiInput& midiInput)
    : midiInput(midiInput)
{
}

void ChordTriggeredEnvelopeFilter::setNotes(const std::set<uint8_t>& notes)
{
    chordNotes.clear();
    for (auto note : notes)
        chordNotes[note] = false;
}

void ChordTriggeredEnvelopeFilter::activate()
{
    midiInput.subscribe(*this);
}

void ChordTriggeredEnvelopeFilter::deactivate()
{
    midiInput.unsubscribe(*this);
    for (auto& [note, pressed] : chordNotes)
        pressed = false;
}

void ChordTriggeredEnvelopeFilter::execute(processing::RgbStrip& strip,
                                           const ProcessingBlock::Input& input)
{
    scheduler.executeAll();

    calculateTrigger(input.nowMs);
    render(strip, input.nowMs);
}

ProcessingBlock::Mode ChordTriggeredEnvelopeFilter::mode() const
{
    return Mode::overwriting;
}

void ChordTriggeredEnvelopeFilter::onNoteChange(uint8_t channel, uint8_t pitch, uint8_t velocity,
                                                bool on)
{
    scheduler.schedule(
        [this, channel, pitch, on]
        {
            if (channel != this->channel)
                return;

            LOG_DEBUG_PARAMS("received note %u: %s", pitch, on ? "on" : "off");
            if (chordNotes.find(pitch) == chordNotes.end())
                return;

            chordNotes[pitch] = on;
        });
}

void ChordTriggeredEnvelopeFilter::calculateTrigger(uint32_t now)
{
    if (std::all_of(chordNotes.cbegin(), chordNotes.cend(),
                    [](const auto& pair) { return pair.second; }))
    {
        if (triggerTimeMs.has_value() && !canRestart)
            return;
        triggerTimeMs = now;
        LOG_DEBUG("triggered");
    }
    else if (triggerTimeMs.has_value() && envelope.completed(now, *triggerTimeMs))
    {
        triggerTimeMs.reset();
        LOG_DEBUG("removed trigger");
    }
}

void ChordTriggeredEnvelopeFilter::render(processing::RgbStrip& strip, uint32_t now)
{
    if (triggerTimeMs.has_value())
    {
        float brightness = envelope.calculateProgress(now, *triggerTimeMs);
        for (auto& color : strip)
            color *= brightness;
    }
    else
    {
        for (auto& color : strip)
            color = processing::color_constants::off;
    }
}
