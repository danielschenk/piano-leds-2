#include <gtest/gtest.h>

#include "../Mock/MockProcessingBlockFactory.hpp"
#include "../Mock/MockProcessingChain.hpp"
#include "../Patch.hpp"

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

class PatchTest : public ::testing::Test
{
  public:
    PatchTest() : processingBlockFactory(), processingChain(new NiceMock<MockProcessingChain>)
    {
        ON_CALL(processingBlockFactory, createProcessingChain())
            .WillByDefault(Return(processingChain));

        patch = new Patch(processingBlockFactory);
    }

    virtual ~PatchTest()
    {
        delete patch;

        // deleted by Patch
        processingChain = nullptr;
    }

    NiceMock<MockProcessingBlockFactory> processingBlockFactory;
    NiceMock<MockProcessingChain>* processingChain;
    Patch* patch;
};

TEST_F(PatchTest, defaults)
{
    EXPECT_EQ(0, patch->getBank());
    EXPECT_EQ(0, patch->getProgram());
    EXPECT_EQ(false, patch->hasBankAndProgram());
    EXPECT_EQ("Untitled Patch", patch->getName());
}

TEST_F(PatchTest, convertToJson)
{
    // Set some non-defaults
    patch->setBank(42);
    patch->setProgram(43);
    patch->setName("Awesome patch");

    Json::object mockChainJson;
    mockChainJson["objectType"] = "mockChain";
    mockChainJson["someProperty"] = 42;
    EXPECT_CALL(*processingChain, convertToJson()).WillOnce(Return(mockChainJson));

    Json::object converted = patch->convertToJson().object_items();
    EXPECT_EQ(42, converted.at("bank").number_value());
    EXPECT_EQ(43, converted.at("program").number_value());
    EXPECT_EQ(true, converted.at("hasBankAndProgram").bool_value());
    EXPECT_EQ("Awesome patch", converted.at("name").string_value());

    EXPECT_EQ(mockChainJson, converted["processingChain"].object_items());
    EXPECT_EQ("Patch", converted.at("objectType").string_value());
}

TEST_F(PatchTest, convertFromJson)
{
    Json::object mockChainJson;
    mockChainJson["objectType"] = "mockChain";
    mockChainJson["someProperty"] = 42;

    Json::object j;
    j["processingChain"] = mockChainJson;
    j["bank"] = 42;
    j["program"] = 43;
    j["hasBankAndProgram"] = true;
    j["name"] = std::string("Awesome patch");

    EXPECT_CALL(*processingChain, convertFromJson(Json(mockChainJson)));
    patch->convertFromJson(Json(j));

    EXPECT_EQ(42, patch->getBank());
    EXPECT_EQ(43, patch->getProgram());
    EXPECT_EQ(true, patch->hasBankAndProgram());
    EXPECT_EQ("Awesome patch", patch->getName());
}

TEST_F(PatchTest, activate)
{
    EXPECT_CALL(*processingChain, activate());
    patch->activate();
}

TEST_F(PatchTest, deactivate)
{
    EXPECT_CALL(*processingChain, deactivate());
    patch->deactivate();
}

TEST_F(PatchTest, execute)
{
    processing::RgbStrip strip;
    strip.push_back(processing::RgbColor({0, 0, 0}));

    // Pass a map with something we can verify
    processing::TNoteToLightMap map;
    map[42] = 42;

    // Let the mock processing chain do something with the strip which we can verify
    processing::RgbColor valueAfterProcessing({1, 2, 3});
    ASSERT_NE(valueAfterProcessing, strip[0]);

    EXPECT_CALL(*processingChain, execute(_, map))
        .WillOnce(Invoke(
            [valueAfterProcessing](processing::RgbStrip& strip, const processing::TNoteToLightMap&)
            { strip[0] = valueAfterProcessing; }));

    patch->execute(strip, map);

    EXPECT_EQ(valueAfterProcessing, strip[0]);
}
