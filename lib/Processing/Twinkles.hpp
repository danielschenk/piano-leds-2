#ifndef PROCESSING_TWINKLE_HPP
#define PROCESSING_TWINKLE_HPP

#include <map>

#include "ProcessingBlock.hpp"

class MonotonicTime;

namespace processing
{

class Twinkles : public ProcessingBlock
{
  public:
    explicit Twinkles(const MonotonicTime& monotonicTime);
    void execute(RgbStrip& strip, const NoteToLightMap& noteToLightMap) override;

    RgbColor color{RgbColor{0xda, 0xa5, 0x20} * 0.3};
    // TODO instead of fixed interval just keep adding until strip is filled a certain pct?
    static constexpr uint32_t startIntervalMs{100};
    static constexpr uint32_t fadeInMs{1000};
    static constexpr uint32_t fadeOutMs{1000};

  private:
    void addNewTwinkle(std::size_t stripSize, uint32_t now);
    void pruneDeadTwinkles(uint32_t now);
    void render(RgbStrip& strip, uint32_t now);

    const MonotonicTime& monotonicTime;

    uint32_t lastOnTimeMs{0};
    // TODO less realloc-intensive method
    std::map<std::size_t, uint32_t> twinkles;
};

}  // namespace processing

#endif
