#include "../DataClass.hpp"
#include "Mock/LoggingTest.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace processing
{

using ::testing::_;

struct FooProperties : public DataClass<FooProperties>
{
    PROPERTY(FooProperties, int, foo);
    PROPERTY(FooProperties, bool, bar);
};

class DataClassTest : public ::testing::Test, public LoggingTest
{
  public:
    FooProperties properties;
};

TEST_F(DataClassTest, resolve)
{
    properties.set("foo", 42);
    EXPECT_EQ(properties.foo, 42);
}

TEST_F(DataClassTest, resolveWrongType)
{
    properties.foo = 42;
    EXPECT_CALL(mockLoggingTarget, logMessage(_, logging::Level::error, _, _));
    properties.set("foo", "baz");
    EXPECT_EQ(properties.foo, 42);
}

}  // namespace processing
