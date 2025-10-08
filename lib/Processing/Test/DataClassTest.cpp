#include "../DataClass.hpp"
#include "gtest/gtest.h"

namespace processing
{

struct FooProperties : public DataClass<FooProperties>
{
    PROPERTY(FooProperties, int, foo);
    PROPERTY(FooProperties, bool, bar);
};

class DataClassTest : public ::testing::Test
{
  public:
    FooProperties properties;
};

TEST_F(DataClassTest, resolve)
{
    properties.set("foo", 42);
    EXPECT_EQ(properties.foo, 42);
}

}  // namespace processing
