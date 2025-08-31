#include "../NoteRgbSource.h"
#include "../Mock/MockRgbFunction.h"
#include "../Mock/MockRgbFunctionFactory.h"

#include "gtest/gtest.h"
#include "Test/MidiInputObserverTest.h"
#include "Mock/LoggingTest.h"
#include "Mock/MockTime.h"


using ::testing::_;
using ::testing::SaveArg;
using ::testing::Return;
using ::testing::HasSubstr;
using ::testing::Each;
using ::testing::InSequence;
using ::testing::AnyNumber;

#define LOGGING_COMPONENT "NoteRgbSource"

ACTION(ReturnFullWhiteWhenSounding)
{
    Processing::TRgb output;
    if(arg0.sounding)
    {
        output.r = 0xff;
        output.g = 0xff;
        output.b = 0xff;
    }

    return output;
}

ACTION(ReturnMinimalWhiteWhenSounding)
{
    Processing::TRgb output;
    if(arg0.sounding)
    {
        output.r = 0x01;
        output.g = 0x01;
        output.b = 0x01;
    }

    return output;
}

class NoteRgbSourceTest
    : public LoggingTest
    , public MidiInputObserverTest
    , public ::testing::Test
{
public:
    static constexpr size_t stripSize = 10;

    NoteRgbSourceTest()
        : strip(stripSize)
        , exampleJson(R"(
             {
                 "objectType": "NoteRgbSource",
                 "channel": 6,
                 "usingPedal": false,
                 "rgbFunction": {
                     "objectType": "MockRgbFunction",
                     "someParameter": 42
                 }
             })")
        , noteRgbSource(mockMidiInput, mockRgbFunctionFactory, mockTime)
    {
        for(int i = 0; i < stripSize; ++i)
        {
            // Default: simple 1-to-1 mapping
            noteToLightMap[i] = i;
        }

        auto rgbFunction(std::make_shared<NiceMock<MockRgbFunction>>());
        ON_CALL(*rgbFunction, calculate(_, _)).WillByDefault(ReturnFullWhiteWhenSounding());
        noteRgbSource.setRgbFunction(rgbFunction);
        noteRgbSource.activate();
    }

    void resetStrip()
    {
        for(auto& color : strip)
        {
            color.r = 0;
            color.g = 0;
            color.b = 0;
        }
    }

    MockRgbFunctionFactory mockRgbFunctionFactory;
    NiceMock<MockTime> mockTime;
    NoteRgbSource noteRgbSource;
    Processing::TRgbStrip strip;

    Processing::TNoteToLightMap noteToLightMap;

    std::string exampleJson;
};

TEST_F(NoteRgbSourceTest, noNotesSounding)
{
    strip[0] = {4, 5, 6};
    strip[6] = {7, 8, 9};
    strip[stripSize-1] = {11, 12, 13};

    noteRgbSource.execute(strip, noteToLightMap);

    EXPECT_THAT(strip, Each(Processing::TRgb({0, 0, 0})));
}

TEST_F(NoteRgbSourceTest, noteOn)
{
    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    noteRgbSource.execute(strip, noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto expected = Processing::TRgbStrip(stripSize);
    expected[0] = {0xff, 0xff, 0xff};
    expected[5] = {0xff, 0xff, 0xff};

    EXPECT_EQ(expected, strip);
}

TEST_F(NoteRgbSourceTest, noteOnOverwritesAlreadyEnabledLed)
{
    auto rgbFunction(std::make_shared<NiceMock<MockRgbFunction>>());
    ON_CALL(*rgbFunction, calculate(_, _)).WillByDefault(ReturnMinimalWhiteWhenSounding());
    noteRgbSource.setRgbFunction(rgbFunction);

    std::fill(strip.begin(), strip.end(), Processing::TRgb{0xff, 0xff, 0xff});

    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    noteRgbSource.execute(strip, noteToLightMap);

    Processing::TRgbStrip expected(strip);
    expected[0] = {0x01, 0x01, 0x01};
    expected[5] = {0x01, 0x01, 0x01};

    EXPECT_EQ(expected, strip);
}

TEST_F(NoteRgbSourceTest, deactivateDisablesAllNotes)
{
    // (channel, number, velocity, on/off)
    noteRgbSource.deactivate();
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    noteRgbSource.execute(strip, noteToLightMap);

    EXPECT_THAT(strip, Each(Processing::TRgb({0, 0, 0})));
}

TEST_F(NoteRgbSourceTest, noteOff)
{
    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    observer->onNoteChange(0, 0, 8, false);

    noteRgbSource.execute(strip, noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto expected = Processing::TRgbStrip(stripSize);
    expected[5] = {0xff, 0xff, 0xff};

    EXPECT_EQ(expected, strip);
}

TEST_F(NoteRgbSourceTest, ignoreOtherChannel)
{
    observer->onNoteChange(1, 0, 1, true);

    noteRgbSource.execute(strip, noteToLightMap);
    EXPECT_THAT(strip, Each(Processing::TRgb({0, 0, 0})));
}

TEST_F(NoteRgbSourceTest, ignorePedal)
{
    noteRgbSource.setUsingPedal(false);

    observer->onNoteChange(0, 0, 1, true);
    observer->onControlChange(0, IMidiInterface::DAMPER_PEDAL, 0xff);
    observer->onNoteChange(0, 0, 1, false);

    noteRgbSource.execute(strip, noteToLightMap);
    EXPECT_THAT(strip, Each(Processing::TRgb({0, 0, 0})));
}

TEST_F(NoteRgbSourceTest, usePedal)
{
    // Press a key
    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    // Press pedal
    // (channel, number, value)
    observer->onControlChange(0, IMidiInterface::DAMPER_PEDAL, 0xff);
    // Press another key
    observer->onNoteChange(0, 2, 1, true);

    auto expected = Processing::TRgbStrip(stripSize);
    // Both notes are still sounding
    expected[0] = {0xff, 0xff, 0xff};
    expected[2] = {0xff, 0xff, 0xff};
    noteRgbSource.execute(strip, noteToLightMap);
    EXPECT_EQ(expected, strip);

    // Release keys
    observer->onNoteChange(0, 0, 1, false);
    observer->onNoteChange(0, 2, 1, false);

    // Both notes are still sounding
    resetStrip();
    noteRgbSource.execute(strip, noteToLightMap);
    EXPECT_EQ(expected, strip);

    // Release pedal
    observer->onControlChange(0, IMidiInterface::DAMPER_PEDAL, 0);

    // Notes are not sounding anymore
    expected[0] = {0, 0, 0};
    expected[2] = {0, 0, 0};
    resetStrip();
    noteRgbSource.execute(strip, noteToLightMap);
    EXPECT_EQ(expected, strip);
}

/** Action definition for mock RGB function. */
ACTION(ReturnBlueWhenNoteSoundingOtherwiseRed)
{
    Processing::TRgb output;
    if(arg0.sounding)
    {
        output.b = 1;
    }
    else
    {
        output.r = 1;
    }

    return output;
}

TEST_F(NoteRgbSourceTest, otherRgbFunction)
{
    auto mockRgbFunction = std::make_shared<MockRgbFunction>();
    EXPECT_CALL(*mockRgbFunction, calculate(_, _))
        .WillRepeatedly(ReturnBlueWhenNoteSoundingOtherwiseRed());
    noteRgbSource.setRgbFunction(mockRgbFunction);

    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    noteRgbSource.execute(strip, noteToLightMap);

    auto expected = Processing::TRgbStrip(stripSize);
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

TEST_F(NoteRgbSourceTest, timePassedToRgbFunction)
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
        EXPECT_CALL(*mockRgbFunction, calculate(_, 44))
            .Times(AnyNumber());
        noteRgbSource.setRgbFunction(mockRgbFunction);

        noteRgbSource.execute(strip, noteToLightMap);
    }
}

TEST_F(NoteRgbSourceTest, otherNoteToLightMap)
{
    noteToLightMap.clear();
    noteToLightMap[0] = 9;
    noteToLightMap[5] = 8;

    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 5, 6, true);

    noteRgbSource.execute(strip, noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto expected = Processing::TRgbStrip(stripSize);
    expected[9] = {0xff, 0xff, 0xff};
    expected[8] = {0xff, 0xff, 0xff};

    EXPECT_EQ(expected, strip);
}

TEST_F(NoteRgbSourceTest, doNotWriteOutsideStrip)
{
    // (channel, number, velocity, on/off)
    observer->onNoteChange(0, 0, 1, true);
    observer->onNoteChange(0, 9, 6, true);

    auto shorterStrip = Processing::TRgbStrip(5);
    noteRgbSource.execute(shorterStrip, noteToLightMap);

    // Default: white, factor 255, so any velocity >0 will cause full on
    auto expected = Processing::TRgbStrip(5);
    expected[0] = {0xff, 0xff, 0xff};

    EXPECT_EQ(expected, shorterStrip);
}

TEST_F(NoteRgbSourceTest, deleteRgbFunction)
{
    auto mock1 = std::make_shared<MockRgbFunction>();
    auto mock2 = std::make_shared<MockRgbFunction>();

    // Need to set an action, to make Google Test throw an error in case of a leaked mock.
    ON_CALL(*mock1, calculate(_, _))
        .WillByDefault(Return(Processing::TRgb()));
    ON_CALL(*mock2, calculate(_, _))
        .WillByDefault(Return(Processing::TRgb()));

    noteRgbSource.setRgbFunction(mock1);
    noteRgbSource.setRgbFunction(mock2);
}

TEST_F(NoteRgbSourceTest, convertToJson)
{
    auto mockRgbFunction = std::make_shared<MockRgbFunction>();
    ASSERT_NE(nullptr, mockRgbFunction);

    Json::object mockRgbFunctionJson;
    mockRgbFunctionJson["objectType"] = "MockRgbFunction";
    mockRgbFunctionJson["someParameter"] = 42;
    EXPECT_CALL(*mockRgbFunction, convertToJson())
        .WillOnce(Return(mockRgbFunctionJson));

    // Set some non-default values
    noteRgbSource.setRgbFunction(mockRgbFunction);
    noteRgbSource.setChannel(6);
    noteRgbSource.setUsingPedal(false);

    Json::object j = noteRgbSource.convertToJson().object_items();
    EXPECT_EQ("NoteRgbSource", j.at("objectType").string_value());
    EXPECT_EQ(6, j.at("channel").int_value());
    EXPECT_EQ(false, j.at("usingPedal").bool_value());
    EXPECT_EQ("MockRgbFunction", j.at("rgbFunction").object_items().at("objectType").string_value());
    EXPECT_EQ(42, j.at("rgbFunction").object_items().at("someParameter").int_value());
}

TEST_F(NoteRgbSourceTest, convertFromJson)
{
    std::string err;
    Json j(Json::parse(exampleJson, err, json11::STANDARD));
    Json::object mockRgbFunctionJson;
    mockRgbFunctionJson["objectType"] = "MockRgbFunction";
    mockRgbFunctionJson["someParameter"] = 42;

    auto mockRgbFunction = std::make_shared<MockRgbFunction>();
    ASSERT_NE(nullptr, mockRgbFunction);
    EXPECT_CALL(*mockRgbFunction, calculate(_, _))
        .WillRepeatedly(Return(Processing::TRgb(1, 2, 3)));

    EXPECT_CALL(mockRgbFunctionFactory, createRgbFunction(Json(mockRgbFunctionJson)))
        .WillOnce(Return(mockRgbFunction));

    noteRgbSource.convertFromJson(j);
    EXPECT_EQ(6, noteRgbSource.getChannel());
    EXPECT_EQ(false, noteRgbSource.isUsingPedal());

    Processing::TRgbStrip expected(3);
    expected[0] = {1, 2, 3};
    expected[1] = {1, 2, 3};
    expected[2] = {1, 2, 3};
    Processing::TRgbStrip testStrip(3);
    noteRgbSource.execute(testStrip, noteToLightMap);
    EXPECT_EQ(expected, testStrip);
}
