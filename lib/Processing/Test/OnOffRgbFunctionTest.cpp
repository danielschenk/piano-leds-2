#include "../OnOffRgbFunction.hpp"
#include "gtest/gtest.h"

namespace processing
{

class OnOffRgbFunctionTest : public ::testing::Test
{
  public:
    OnOffRgbFunction function{color_constants::blue * 0.8};
    NoteState noteState;
};

TEST_F(OnOffRgbFunctionTest, notSounding)
{
    noteState.sounding = false;
    EXPECT_EQ(function.calculate(noteState, 0), color_constants::off);
}

TEST_F(OnOffRgbFunctionTest, sounding)
{
    noteState.sounding = true;
    for (uint8_t velocity = 1; velocity < 128; ++velocity)
    {
        noteState.pressDownVelocity = velocity;
        EXPECT_EQ(function.calculate(noteState, 0), function.color);
    }
}

}  // namespace processing
