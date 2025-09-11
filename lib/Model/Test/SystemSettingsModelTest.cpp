#include "../SystemSettingsModel.h"

#include <gtest/gtest.h>

class SystemSettingsModelTest : public testing::Test
{
public:
    SystemSettingsModelTest() = default;
    SystemSettingsModel model;
};

TEST_F(SystemSettingsModelTest, defaults)
{
    EXPECT_EQ("PianoLeds", model.getWifiAPSsid());
    EXPECT_EQ("LedsFlashSomeNotes", model.getWifiAPPassword());
}

TEST_F(SystemSettingsModelTest, wifiAPSsid)
{
    bool observerCalled = false;
    // model.subscribe([&](){observerCalled = true;});

    std::string val("foo");
    model.setWifiAPSsid(val);
    EXPECT_EQ(val, model.getWifiAPSsid());

    // EXPECT_TRUE(observerCalled);
}

TEST_F(SystemSettingsModelTest, wifiAPPassword)
{
    bool observerCalled = false;
    // model.subscribe([&](){observerCalled = true;});

    std::string val("foo");
    model.setWifiAPPassword(val);
    EXPECT_EQ(val, model.getWifiAPPassword());

    // EXPECT_TRUE(observerCalled);
}
