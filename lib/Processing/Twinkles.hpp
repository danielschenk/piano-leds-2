#ifndef PROCESSING_TWINKLE_HPP
#define PROCESSING_TWINKLE_HPP

#include <cstddef>
#include <optional>
#include <vector>

#include "ProcessingBlock.hpp"
#include "TimeEnvelope.hpp"

namespace processing
{

class Twinkles : public ProcessingBlock
{
  public:
    Twinkles() = default;

    // ProcessingBlock
    void execute(RgbStrip& strip, const Input& input) override;
    void deactivate() override;

    RgbColor color{color_constants::goldenrod * 0.3f};
    // TODO instead of fixed interval just keep adding until strip is filled a certain pct
    // This allows to control density independently of strip size and fade times
    static constexpr uint32_t spawnIntervalMs{100};
    TimeEnvelope envelope{1000, 1000};

  private:
    void spawnTwinkle(std::size_t stripSize, uint32_t now);
    void pruneDeadTwinkles(uint32_t now);
    void render(RgbStrip& strip, uint32_t now);

    uint32_t lastSpawnTimeMs{0};

    struct Twinkle
    {
        uint32_t spawnTimeMs;
    };
    std::vector<std::optional<Twinkle>> twinkles;
};

}  // namespace processing

#endif
