#ifndef PROCESSING_CHORDTIRGGEREDATTACKRELEASEFILTER_HPP
#define PROCESSING_CHORDTIRGGEREDATTACKRELEASEFILTER_HPP

#include <cstdint>
#include <set>

#include "ProcessingBlock.hpp"
class ChordTriggeredAttackReleaseFilter : public ProcessingBlock
{
  public:
    ChordTriggeredAttackReleaseFilter() = default;

    std::set<uint8_t> chordNoteNumbers;
    uint32_t attackMs{0};
    uint32_t releaseMs{1000};

    // ProcessingBlock
    void execute(processing::RgbStrip& strip, const Input& input) override;
    Mode mode() const override;

  private:
    std::optional<uint32_t> triggerTimeMs;
};

#endif
