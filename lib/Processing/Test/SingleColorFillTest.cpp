#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "../SingleColorFill.hpp"
#include "Json11Helper.hpp"
#include "LoggingEntryPoint.hpp"
#include "Mock/MockMonotonicTime.hpp"

using ::testing::NiceMock;

class SingleColorFillTest : public ::testing::Test
{
  public:
    SingleColorFillTest() : time(), source()
    {
        LoggingEntryPoint::setTime(&time);
    }

    NiceMock<MockMonotonicTime> time;
    SingleColorFill source;
};

TEST_F(SingleColorFillTest, executeDifferentColors)
{
    processing::RgbStrip strip(20);
    std::vector<processing::RgbColor> colors({{0, 0, 0}, {255, 255, 255}, {1, 2, 3}});

    for (const auto& colorIt : colors)
    {
        source.color = colorIt;
        source.execute(strip, processing::NoteToLightMap());
        for (const auto& outputIt : strip)
        {
            EXPECT_EQ(outputIt, colorIt);
        }
    }
}
TEST_F(SingleColorFillTest, convertFromJson)
{
    std::string err;
    Json j = Json::parse(R"(
        {
            "objectType": "SingleColorFill",
            "r": 10,
            "g": 20,
            "b": 30
        }
        )",
                         err, json11::STANDARD);

    source.convertFromJson(j);
    EXPECT_EQ(*source.color, processing::RgbColor({10, 20, 30}));
}

TEST_F(SingleColorFillTest, convertFromJsonWithWrongType)
{
    std::string err;
    Json j = Json::parse(R"(
        {
            "objectType": "wrong",
            "r": 10,
            "g": 20,
            "b": 30
        }
        )",
                         err, json11::STANDARD);

    source.convertFromJson(j);
    EXPECT_EQ(*source.color, processing::RgbColor({10, 20, 30}));
}

TEST_F(SingleColorFillTest, convertFromJsonWithMissingColor)
{
    std::string err;
    Json j = Json::parse(R"(
        {
            "objectType": "SingleColorFill",
            "r": 10,
            "b": 30
        }
        )",
                         err, json11::STANDARD);

    source.convertFromJson(j);
    EXPECT_EQ(*source.color, processing::RgbColor({10, 0, 30}));
}

TEST_F(SingleColorFillTest, convertToJson)
{
    source.color = processing::RgbColor({40, 50, 60});

    Json::object j = source.convertToJson().object_items();
    EXPECT_EQ(4, j.size());
    EXPECT_EQ("SingleColorFill", j.at("objectType").string_value());
    EXPECT_EQ(40, j.at("r").number_value());
    EXPECT_EQ(50, j.at("g").number_value());
    EXPECT_EQ(60, j.at("b").number_value());
}
