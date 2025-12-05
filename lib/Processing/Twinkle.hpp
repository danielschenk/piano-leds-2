#ifndef PROCESSING_TWINKLE_HPP
#define PROCESSING_TWINKLE_HPP

#include <map>

#include "ProcessingBlock.hpp"

class MonotonicTime;

namespace processing
{

class Twinkle : public ProcessingBlock
{
  public:
    explicit Twinkle(const MonotonicTime& monotonicTime);
    void execute(RgbStrip& strip, const NoteToLightMap& noteToLightMap) override;

    RgbColor color{color_constants::white};
    static constexpr uint32_t startIntervalMs{200};
    static constexpr uint32_t fadeInMs{500};
    static constexpr uint32_t fadeOutMs{500};

  private:
    void addNewTwinkle(std::size_t stripSize, uint32_t now);
    void pruneDeadTwinkles(uint32_t now);
    void render(RgbStrip& strip, uint32_t now);

    const MonotonicTime& monotonicTime;

    uint32_t lastOnTimeMs{0};
    std::map<std::size_t, uint32_t> twinkles;
};

}  // namespace processing

#endif
