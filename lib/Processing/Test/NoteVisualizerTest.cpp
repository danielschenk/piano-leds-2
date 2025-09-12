#include "../Mock/MockRgbFunction.hpp"
#include "../Mock/MockRgbFunctionFactory.hpp"
#include "../NoteVisualizer.hpp"
#include "Mock/LoggingTest.hpp"
#include "Mock/MockTime.hpp"
#include "Test/MidiInputObserverTest.hpp"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Each;
using ::testing::HasSubstr;
using ::testing::InSequence;
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

class NoteVisualizerTest : public LoggingTest, public MidiInputObserverTest, public ::testing::Test
{
  public:
    static constexpr size_t stripSize = 10;

    NoteVisualizerTest()
        : strip(stripSize),
          exampleJson(R"(
             {
                 "objectType": "NoteVisualizer",
                 "channel": 6,
                 "usingPedal": false,
                 "rgbFunction": {
                     "objectType": "MockRgbFunction",
                     "someParameter": 42
                 }
             })"),
          noteVisualizer(mockMidiInput, mockRgbFunctionFactory, mockTime)
    {
        for (int i = 0; i < stripSize; ++i)
        {
            // Default: simple 1-to-1 mapping
            noteToLightMap[i] = i;
        }

        auto rgbFunction(std::make_shared<NiceMock<MockRgbFunction>>());
        ON_CALL(*rgbFunction, calculate(_, _)).WillByDefault(ReturnFullWhiteWhenSounding());
        noteVisualizer.setRgbFunction(rgbFunction);
        noteVisualizer.activate();
    }

    void resetStrip()
    {
        for (auto& color : strip)
        {
            color.r = 0;
            color.g = 0;
            color.b = 0;
        }
    }

    MockRgbFunctionFactory mockRgbFunctionFactory;
    NiceMock<MockTime> mockTime;
    NoteVisualizer noteVisualizer;
    processing::RgbStrip strip;

    processing::TNoteToLightMap noteToLightMap;

    std::string exampleJson;
};

TEST_F(NoteVisualizerTest, noNotesSounding)
{
    strip[0] = {4, 5, 6};
    strip[6] = {7, 8, 9};
    strip[stripSize - 1] = {11, 12, 13};

    noteVisualizer.execute(strip, noteToLightMap);

    EXPECT_THAT(strip, Each(processing::RgbColor({0, 0, 0})));
}

TEST_F(NoteVisualizerTest, noteOn)
{
    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    noteVisualizer.execute(strip, noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto expected = processing::RgbStrip(stripSize);
    expected[0] = {0xff, 0xff, 0xff};
    expected[5] = {0xff, 0xff, 0xff};

    EXPECT_EQ(expected, strip);
}

TEST_F(NoteVisualizerTest, noteOnOverwritesAlreadyEnabledLed)
{
    auto rgbFunction(std::make_shared<NiceMock<MockRgbFunction>>());
    ON_CALL(*rgbFunction, calculate(_, _)).WillByDefault(ReturnMinimalWhiteWhenSounding());
    noteVisualizer.setRgbFunction(rgbFunction);

    std::fill(strip.begin(), strip.end(), processing::RgbColor{0xff, 0xff, 0xff});

    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    noteVisualizer.execute(strip, noteToLightMap);

    processing::RgbStrip expected(strip);
    expected[0] = {0x01, 0x01, 0x01};
    expected[5] = {0x01, 0x01, 0x01};

    EXPECT_EQ(expected, strip);
}

TEST_F(NoteVisualizerTest, deactivateDisablesAllNotes)
{
    // (channel, number, velocity, on/off)
    noteVisualizer.deactivate();
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    noteVisualizer.execute(strip, noteToLightMap);

    EXPECT_THAT(strip, Each(processing::RgbColor({0, 0, 0})));
}

TEST_F(NoteVisualizerTest, noteOff)
{
    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    observer->onNoteChange(0, 0, 8, false);

    noteVisualizer.execute(strip, noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto expected = processing::RgbStrip(stripSize);
    expected[5] = {0xff, 0xff, 0xff};

    EXPECT_EQ(expected, strip);
}

TEST_F(NoteVisualizerTest, ignoreOtherChannel)
{
    observer->onNoteChange(1, 0, 1, true);

    noteVisualizer.execute(strip, noteToLightMap);
    EXPECT_THAT(strip, Each(processing::RgbColor({0, 0, 0})));
}

TEST_F(NoteVisualizerTest, ignorePedal)
{
    noteVisualizer.setUsingPedal(false);

    observer->onNoteChange(0, 0, 1, true);
    observer->onControlChange(0, IMidiInterface::DAMPER_PEDAL, 0xff);
    observer->onNoteChange(0, 0, 1, false);

    noteVisualizer.execute(strip, noteToLightMap);
    EXPECT_THAT(strip, Each(processing::RgbColor({0, 0, 0})));
}

TEST_F(NoteVisualizerTest, usePedal)
{
    // Press a key
    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    // Press pedal
    // (channel, number, value)
    observer->onControlChange(0, IMidiInterface::DAMPER_PEDAL, 0xff);
    // Press another key
    observer->onNoteChange(0, 2, 1, true);

    auto expected = processing::RgbStrip(stripSize);
    // Both notes are still sounding
    expected[0] = {0xff, 0xff, 0xff};
    expected[2] = {0xff, 0xff, 0xff};
    noteVisualizer.execute(strip, noteToLightMap);
    EXPECT_EQ(expected, strip);

    // Release keys
    observer->onNoteChange(0, 0, 1, false);
    observer->onNoteChange(0, 2, 1, false);

    // Both notes are still sounding
    resetStrip();
    noteVisualizer.execute(strip, noteToLightMap);
    EXPECT_EQ(expected, strip);

    // Release pedal
    observer->onControlChange(0, IMidiInterface::DAMPER_PEDAL, 0);

    // Notes are not sounding anymore
    expected[0] = {0, 0, 0};
    expected[2] = {0, 0, 0};
    resetStrip();
    noteVisualizer.execute(strip, noteToLightMap);
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
    auto mockRgbFunction = std::make_shared<MockRgbFunction>();
    EXPECT_CALL(*mockRgbFunction, calculate(_, _))
        .WillRepeatedly(ReturnBlueWhenNoteSoundingOtherwiseRed());
    noteVisualizer.setRgbFunction(mockRgbFunction);

    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    noteVisualizer.execute(strip, noteToLightMap);

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
    EXPECT_CALL(mockTime, getMilliseconds())
        .WillOnce(Return(42))
        .WillOnce(Return(43))
        .WillRepeatedly(Return(44));

    auto mockRgbFunction = std::make_shared<MockRgbFunction>();

    {
        InSequence dummy;
        EXPECT_CALL(*mockRgbFunction, calculate(_, 42));
        EXPECT_CALL(*mockRgbFunction, calculate(_, 43));
        EXPECT_CALL(*mockRgbFunction, calculate(_, 44)).Times(AnyNumber());
        noteVisualizer.setRgbFunction(mockRgbFunction);

        noteVisualizer.execute(strip, noteToLightMap);
    }
}

TEST_F(NoteVisualizerTest, otherNoteToLightMap)
{
    noteToLightMap.clear();
    noteToLightMap[0] = 9;
    noteToLightMap[5] = 8;

    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    noteVisualizer.execute(strip, noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto expected = processing::RgbStrip(stripSize);
    expected[9] = {0xff, 0xff, 0xff};
    expected[8] = {0xff, 0xff, 0xff};

    EXPECT_EQ(expected, strip);
}

TEST_F(NoteVisualizerTest, doNotWriteOutsideStrip)
{
    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 9, 6, true);

    auto shorterStrip = processing::RgbStrip(5);
    noteVisualizer.execute(shorterStrip, noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto expected = processing::RgbStrip(5);
    expected[0] = {0xff, 0xff, 0xff};

    EXPECT_EQ(expected, shorterStrip);
}

TEST_F(NoteVisualizerTest, deleteRgbFunction)
{
    auto mock1 = std::make_shared<MockRgbFunction>();
    auto mock2 = std::make_shared<MockRgbFunction>();

    // Need to set an action, to make Google Test throw an error in case of a leaked mock.
    ON_CALL(*mock1, calculate(_, _)).WillByDefault(Return(processing::RgbColor()));
    ON_CALL(*mock2, calculate(_, _)).WillByDefault(Return(processing::RgbColor()));

    noteVisualizer.setRgbFunction(mock1);
    noteVisualizer.setRgbFunction(mock2);
}

TEST_F(NoteVisualizerTest, convertToJson)
{
    auto mockRgbFunction = std::make_shared<MockRgbFunction>();
    ASSERT_NE(nullptr, mockRgbFunction);

    Json::object mockRgbFunctionJson;
    mockRgbFunctionJson["objectType"] = "MockRgbFunction";
    mockRgbFunctionJson["someParameter"] = 42;
    EXPECT_CALL(*mockRgbFunction, convertToJson()).WillOnce(Return(mockRgbFunctionJson));

    // Set some non-default values
    noteVisualizer.setRgbFunction(mockRgbFunction);
    noteVisualizer.setChannel(6);
    noteVisualizer.setUsingPedal(false);

    Json::object j = noteVisualizer.convertToJson().object_items();
    EXPECT_EQ("NoteVisualizer", j.at("objectType").string_value());
    EXPECT_EQ(6, j.at("channel").int_value());
    EXPECT_EQ(false, j.at("usingPedal").bool_value());
    EXPECT_EQ("MockRgbFunction",
              j.at("rgbFunction").object_items().at("objectType").string_value());
    EXPECT_EQ(42, j.at("rgbFunction").object_items().at("someParameter").int_value());
}

TEST_F(NoteVisualizerTest, convertFromJson)
{
    std::string err;
    Json j(Json::parse(exampleJson, err, json11::STANDARD));
    Json::object mockRgbFunctionJson;
    mockRgbFunctionJson["objectType"] = "MockRgbFunction";
    mockRgbFunctionJson["someParameter"] = 42;

    auto mockRgbFunction = std::make_shared<MockRgbFunction>();
    ASSERT_NE(nullptr, mockRgbFunction);
    EXPECT_CALL(*mockRgbFunction, calculate(_, _))
        .WillRepeatedly(Return(processing::RgbColor(1, 2, 3)));

    EXPECT_CALL(mockRgbFunctionFactory, createRgbFunction(Json(mockRgbFunctionJson)))
        .WillOnce(Return(mockRgbFunction));

    noteVisualizer.convertFromJson(j);
    EXPECT_EQ(6, noteVisualizer.getChannel());
    EXPECT_EQ(false, noteVisualizer.isUsingPedal());

    processing::RgbStrip expected(3);
    expected[0] = {1, 2, 3};
    expected[1] = {1, 2, 3};
    expected[2] = {1, 2, 3};
    processing::RgbStrip testStrip(3);
    noteVisualizer.execute(testStrip, noteToLightMap);
    EXPECT_EQ(expected, testStrip);
}
