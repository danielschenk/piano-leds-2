#include "PianoDecayRgbFunction.h"
#include "gtest/gtest.h"

#include <vector>

class PianoDecayRgbFunctionTest : public testing::Test
{
public:
    struct TTruthTableEntry
    {
        Processing::TTime time;
        Processing::TRgb result;
    };

    PianoDecayRgbFunctionTest()
        : function()
    {
    }

    // Unit under test
    PianoDecayRgbFunction function;
};

TEST_F(PianoDecayRgbFunctionTest, decay)
{
    std::vector<TTruthTableEntry> truthTable = {
            {0, {200, 100, 100}},
            {600, {150, 75, 75}},
            {1200, {100, 50, 50}},
            {8100, {50, 25, 25}},
            {15000, {0, 0, 0}},
    };

    const Processing::TNoteState noteState(true, true, 100, 0);

    function.setRedConstants({2, 0});
    function.setGreenConstants({1, 0});
    function.setBlueConstants({1, 0});

    for(const auto& entry : truthTable)
    {
        EXPECT_EQ(entry.result, function.calculate(noteState, entry.time))
            << "(time " << entry.time << ")";
    }
}

TEST_F(PianoDecayRgbFunctionTest, notSounding)
{
    const Processing::TNoteState noteState(false, false, 127, 0);
    EXPECT_EQ(Processing::TRgb(0, 0, 0), function.calculate(noteState, 42));
}

TEST_F(PianoDecayRgbFunctionTest, convertToJson)
{
    function.setRedConstants({42, 43});
    function.setGreenConstants({44, 45});
    function.setBlueConstants({46, 47});

    auto converted = function.convertToJson().object_items();
    EXPECT_STREQ("PianoDecayRgbFunction", converted.at("objectType").string_value().c_str());
    EXPECT_EQ(42, converted.at("rFactor").number_value());
    EXPECT_EQ(43, converted.at("rOffset").number_value());
    EXPECT_EQ(44, converted.at("gFactor").number_value());
    EXPECT_EQ(45, converted.at("gOffset").number_value());
    EXPECT_EQ(46, converted.at("bFactor").number_value());
    EXPECT_EQ(47, converted.at("bOffset").number_value());
}

TEST_F(PianoDecayRgbFunctionTest, convertFromJson)
{
    Json::object j;
    j["rFactor"] = 42;
    j["rOffset"] = 43;
    j["gFactor"] = 44;
    j["gOffset"] = 45;
    j["bFactor"] = 46;
    j["bOffset"] = 47;

    function.convertFromJson(j);

    EXPECT_EQ(Processing::TLinearConstants({42, 43}), function.getRedConstants());
    EXPECT_EQ(Processing::TLinearConstants({44, 45}), function.getGreenConstants());
    EXPECT_EQ(Processing::TLinearConstants({46, 47}), function.getBlueConstants());
}
