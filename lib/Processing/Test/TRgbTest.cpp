#include <gtest/gtest.h>

#include "ProcessingTypes.hpp"

using namespace Processing;

TEST(TRgbTest, defaultConstructor)
{
    TRgb color;
    EXPECT_EQ(0, color.r);
    EXPECT_EQ(0, color.g);
    EXPECT_EQ(0, color.b);
}

TEST(TRgbTest, constructor)
{
    TRgb color(1, 2, 3);
    EXPECT_EQ(1, color.r);
    EXPECT_EQ(2, color.g);
    EXPECT_EQ(3, color.b);
}

TEST(TRgbTest, addition)
{
    TRgb color1(1, 2, 3), color2(10, 20, 30);
    TRgb result(color1 + color2);

    EXPECT_EQ(11, result.r);
    EXPECT_EQ(22, result.g);
    EXPECT_EQ(33, result.b);
}

TEST(TRgbTest, additionWithClipping)
{
    TRgb color1(250, 250, 250), color2(1, 20, 30);
    TRgb result(color1 + color2);

    EXPECT_EQ(251, result.r);
    EXPECT_EQ(255, result.g);
    EXPECT_EQ(255, result.b);
}

TEST(TRgbTest, subtraction)
{
    TRgb color1(11, 22, 33), color2(1, 2, 3);
    TRgb result(color1 - color2);

    EXPECT_EQ(10, result.r);
    EXPECT_EQ(20, result.g);
    EXPECT_EQ(30, result.b);
}

TEST(TRgbTest, subtractionWithClipping)
{
    TRgb color1(5, 5, 5), color2(1, 20, 30);
    TRgb result(color1 - color2);

    EXPECT_EQ(4, result.r);
    EXPECT_EQ(0, result.g);
    EXPECT_EQ(0, result.b);
}

TEST(TRgbTest, multiplication)
{
    TRgb color(10, 20, 30);
    TRgb result(color * 1.5);

    EXPECT_EQ(15, result.r);
    EXPECT_EQ(30, result.g);
    EXPECT_EQ(45, result.b);
}

TEST(TRgbTest, multiplicationWithOther)
{
    TRgb color(10, 20, 40);
    TRgb result(color * TRgb(128, 64, 32));

    EXPECT_EQ(5, result.r);
    EXPECT_EQ(5, result.g);
    EXPECT_EQ(5, result.b);
}

TEST(TRgbTest, multiplicationWithClipping)
{
    TRgb color(10, 20, 30);
    TRgb result(color * 20);

    EXPECT_EQ(200, result.r);
    EXPECT_EQ(255, result.g);
    EXPECT_EQ(255, result.b);
}
