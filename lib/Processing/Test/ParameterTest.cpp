#include "../Color.hpp"
#include "../Parameter.hpp"
#include "gtest/gtest.h"

namespace processing
{

using ::testing::Test;

struct ParameterTest : public Test
{
    Parameter<int> parameter;
    std::shared_ptr<Parameter<int>::ModulationBinding> binding{
        std::make_shared<Parameter<int>::ModulationBinding>()};

    ParameterTest()
    {
        binding->inputRange = {10, 110};
        binding->outputRange = {200, 2200};
        parameter.modulationBinding = binding;
    }
};

TEST_F(ParameterTest, modulateClipping)
{
    parameter.modulate(0);
    EXPECT_EQ(binding->outputRange.first, parameter.current);

    parameter.modulate(1000);
    EXPECT_EQ(binding->outputRange.second, parameter.current);
}

TEST_F(ParameterTest, modulate)
{
    parameter.modulate(10);
    EXPECT_EQ(binding->outputRange.first, parameter.current);

    parameter.modulate(110);
    EXPECT_EQ(binding->outputRange.second, parameter.current);

    parameter.modulate(60);
    EXPECT_EQ(1200, parameter.current);
    parameter.modulate(65);
    EXPECT_EQ(1300, parameter.current);
}

TEST(ComplexParameterTest, modulateComplexType)
{
    Parameter<RgbColor> parameter;
    std::shared_ptr<Parameter<RgbColor>::ModulationBinding> binding{
        std::make_shared<Parameter<RgbColor>::ModulationBinding>()};

    using namespace color_constants;
    binding->inputRange = {0, 100};
    binding->outputRange = {red, blue};
    parameter.modulationBinding = binding;

    parameter.modulate(0);
    EXPECT_EQ(parameter.current, red);

    parameter.modulate(100);
    EXPECT_EQ(parameter.current, blue);

    parameter.modulate(50);
    EXPECT_EQ(parameter.current, magenta * 0.5f);
}

}  // namespace processing
