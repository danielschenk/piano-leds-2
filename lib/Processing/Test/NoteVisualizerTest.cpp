#include <algorithm>

#include "../Mock/MockRgbFunction.hpp"
#include "../NoteVisualizer.hpp"
#include "Mock/LoggingTest.hpp"
#include "gtest/gtest.h"
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Each;
using ::testing::HasSubstr;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SaveArg;

#define LOGGING_COMPONENT "NoteVisualizer"

ACTION(ReturnFullWhiteWhenSounding)
{
    processing::RgbColor output;
    if (arg0.sounding)
    {
        output.r = 0xff;
        output.g = 0xff;
        output.b = 0xff;
    }

    return output;
}

ACTION(ReturnMinimalWhiteWhenSounding)
{
    processing::RgbColor output;
    if (arg0.sounding)
    {
        output.r = 0x01;
        output.g = 0x01;
        output.b = 0x01;
    }

    return output;
}

class NoteVisualizerTest : public LoggingTest, public ::testing::Test
{
  public:
    static constexpr size_t stripSize = 10;

    NoteVisualizerTest() : strip(stripSize)
    {
        for (int i = 0; i < stripSize; ++i)
        {
            // Default: simple 1-to-1 mapping
            noteToLightMap[i] = i;
        }

        auto rgbFunction(std::make_shared<NiceMock<processing::MockRgbFunction>>());
        ON_CALL(*rgbFunction, calculate(_, _)).WillByDefault(ReturnFullWhiteWhenSounding());
        noteVisualizer.rgbFunction = rgbFunction;
        noteVisualizer.activate();
    }

    void resetStrip()
    {
        std::fill(strip.begin(), strip.end(), processing::color_constants::off);
    }

    void soundNote(MidiInterface::NoteNumber number, MidiInterface::Velocity velocity)
    {
        noteStates[number].pressed = true;
        noteStates[number].sounding = true;
        noteStates[number].pressDownVelocity = velocity;
    }

    NoteVisualizer noteVisualizer;
    processing::RgbStrip strip;

    processing::NoteToLightMap noteToLightMap;
    processing::NoteStates noteStates;
    ProcessingBlock::Input input{0, noteToLightMap, {noteStates}};

    std::string exampleJson;
};

TEST_F(NoteVisualizerTest, noNotesSounding)
{
    strip[0] = {4, 5, 6};
    strip[6] = {7, 8, 9};
    strip[stripSize - 1] = {11, 12, 13};

    noteVisualizer.execute(strip, input);

    EXPECT_THAT(strip, Each(processing::color_constants::off));
}

TEST_F(NoteVisualizerTest, noteOn)
{
    soundNote(0, 1);
    soundNote(5, 6);

    noteVisualizer.execute(strip, input);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto expected = processing::RgbStrip(stripSize);
    expected[0] = {0xff, 0xff, 0xff};
    expected[5] = {0xff, 0xff, 0xff};

    EXPECT_EQ(expected, strip);
}

TEST_F(NoteVisualizerTest, noteOnOverwritesAlreadyEnabledLed)
{
    auto rgbFunction(std::make_shared<NiceMock<processing::MockRgbFunction>>());
    ON_CALL(*rgbFunction, calculate(_, _)).WillByDefault(ReturnMinimalWhiteWhenSounding());
    noteVisualizer.rgbFunction = rgbFunction;

    std::fill(strip.begin(), strip.end(), processing::color_constants::white);

    soundNote(0, 1);
    soundNote(5, 6);

    noteVisualizer.execute(strip, input);

    processing::RgbStrip expected(strip);
    expected[0] = {0x01, 0x01, 0x01};
    expected[5] = {0x01, 0x01, 0x01};

    EXPECT_EQ(expected, strip);
}

/** Action definition for mock RGB function. */
ACTION(ReturnBlueWhenNoteSoundingOtherwiseRed)
{
    processing::RgbColor output;
    if (arg0.sounding)
    {
        output.b = 1;
    }
    else
    {
        output.r = 1;
    }

    return output;
}

TEST_F(NoteVisualizerTest, otherRgbFunction)
{
    auto mockRgbFunction = std::make_shared<processing::MockRgbFunction>();
    EXPECT_CALL(*mockRgbFunction, calculate(_, _))
        .WillRepeatedly(ReturnBlueWhenNoteSoundingOtherwiseRed());
    noteVisualizer.rgbFunction = mockRgbFunction;

    soundNote(0, 1);
    soundNote(5, 6);

    noteVisualizer.execute(strip, input);

    auto expected = processing::RgbStrip(stripSize);
    expected[0] = {0, 0, 1};
    expected[1] = {1, 0, 0};
    expected[2] = {1, 0, 0};
    expected[3] = {1, 0, 0};
    expected[4] = {1, 0, 0};
    expected[5] = {0, 0, 1};
    expected[6] = {1, 0, 0};
    expected[7] = {1, 0, 0};
    expected[8] = {1, 0, 0};
    expected[9] = {1, 0, 0};

    EXPECT_EQ(expected, strip);
}

TEST_F(NoteVisualizerTest, timePassedToRgbFunction)
{
    auto mockRgbFunction = std::make_shared<processing::MockRgbFunction>();
    noteVisualizer.rgbFunction = mockRgbFunction;

    InSequence dummy;
    for (processing::Timestamp t = 42; t < 45; ++t)
    {
        EXPECT_CALL(*mockRgbFunction, calculate(_, t)).Times(stripSize);

        input.nowMs = t;
        noteVisualizer.execute(strip, input);
    }
}

TEST_F(NoteVisualizerTest, otherNoteToLightMap)
{
    noteToLightMap.clear();
    noteToLightMap[0] = 9;
    noteToLightMap[5] = 8;

    soundNote(0, 1);
    soundNote(5, 6);

    noteVisualizer.execute(strip, input);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto expected = processing::RgbStrip(stripSize);
    expected[9] = {0xff, 0xff, 0xff};
    expected[8] = {0xff, 0xff, 0xff};

    EXPECT_EQ(expected, strip);
}

TEST_F(NoteVisualizerTest, doNotWriteOutsideStrip)
{
    soundNote(0, 1);
    soundNote(9, 6);

    auto shorterStrip = processing::RgbStrip(5);
    noteVisualizer.execute(shorterStrip, input);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto expected = processing::RgbStrip(5);
    expected[0] = {0xff, 0xff, 0xff};

    EXPECT_EQ(expected, shorterStrip);
}

TEST_F(NoteVisualizerTest, valid)
{
    EXPECT_TRUE(noteVisualizer.valid(input));
    noteVisualizer.midiSlot = 1;
    EXPECT_FALSE(noteVisualizer.valid(input));
}
