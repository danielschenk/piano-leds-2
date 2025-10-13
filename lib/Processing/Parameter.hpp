#ifndef PROCESSING_PARAMETER_HPP
#define PROCESSING_PARAMETER_HPP

#include <algorithm>
#include <cstdint>
#include <memory>
#include <tuple>

namespace processing
{

struct Modulatable
{
    virtual ~Modulatable() = default;
    virtual void modulate(uint16_t input) = 0;
};

template <typename T>
struct Parameter : public Modulatable
{
    Parameter() = default;
    Parameter(T value) : current(value) {}

    T current;

    struct ModulationBinding
    {
        std::pair<uint16_t, uint16_t> inputRange;
        std::pair<T, T> outputRange;
    };
    std::shared_ptr<ModulationBinding> modulationBinding;

    void modulate(uint16_t input) override
    {
        if (!modulationBinding)
            return;

        auto& inputRange = modulationBinding->inputRange;
        auto& outputRange = modulationBinding->outputRange;
        input = std::min(input, inputRange.second);
        input = std::max(input, inputRange.first);

        float inputFactor =
            (float(input) - inputRange.first) / (float(inputRange.second) - inputRange.first);
        current = (1.0f - inputFactor) * outputRange.first + inputFactor * outputRange.second;
    }
};

}  // namespace processing

#endif
