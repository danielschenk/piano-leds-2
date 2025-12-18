#ifndef LIB_PROCESSING_PIANODECAYRGBFUNCTION_H_
#define LIB_PROCESSING_PIANODECAYRGBFUNCTION_H_

#include <stdint.h>

#include "LinearRgbFunction.hpp"

namespace processing
{

/**
 * RGB function which slowly dims sounding notes to visually mimic the sound of piano strings.
 */
class PianoDecayRgbFunction : public LinearRgbFunction
{
  public:
    using LinearRgbFunction::LinearRgbFunction;

    PianoDecayRgbFunction(const PianoDecayRgbFunction&) = delete;
    PianoDecayRgbFunction& operator=(const PianoDecayRgbFunction&) = delete;

    // RgbFunction implementation
    RgbColor calculate(const NoteState& noteState, Timestamp currentTime) const override;

  protected:
    // RgbFunction implementation
    std::string getObjectType() const override;

  private:
    static constexpr uint32_t fastDecayDurationMs = 1200;
    static constexpr float fastDecayFactor = 0.5f;

    static constexpr uint32_t slowDecayDurationMs = 13800;
    static constexpr float slowDecayFactor = 0.5f;
};

}  // namespace processing

#endif /* LIB_PROCESSING_PIANODECAYRGBFUNCTION_H_ */
