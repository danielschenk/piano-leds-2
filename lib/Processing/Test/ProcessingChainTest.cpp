#include <string>
#include <vector>
#include <gtest/gtest.h>

#include <Mock/LoggingTest.h>

#include "ProcessingBlockContainerTest.h"
#include "ProcessingChain.h"
#include "ProcessingTypes.h"
#include "../Interfaces/IProcessingBlock.h"

#define LOGGING_COMPONENT "ProcessingChain"

using ::testing::Return;
using ::testing::HasSubstr;
using ::testing::ElementsAre;

class ProcessingChainTest
    : public ProcessingBlockContainerTest
    , public ::testing::Test
{
public:
    ProcessingChainTest()
        : processingChain(processingBlockFactory)
    {
        processingChain.activate();
        map[0] = 0;
        map[1] = 1;
        map[2] = 2;
    }

    ProcessingChain processingChain;
    Processing::TNoteToLightMap map;
};

TEST_F(ProcessingChainTest, empty)
{
    auto testStrip = Processing::TRgbStrip(stripSize);
    testStrip[0] = { 1, 0, 0 };
    testStrip[1] = { 0, 1, 0 };
    testStrip[2] = { 0, 0, 1 };

    processingChain.execute(testStrip, Processing::TNoteToLightMap());
    // strip is still zero
    EXPECT_EQ(strip, testStrip);
}

TEST_F(ProcessingChainTest, insertOne)
{
    processingChain.insertBlock(redSource);
    redSource = nullptr;

    auto reference = Processing::TRgbStrip(stripSize);
    reference[0] = { 10, 0, 0 };
    reference[1] = { 10, 0, 0 };
    reference[2] = { 10, 0, 0 };

    processingChain.execute(strip, Processing::TNoteToLightMap());
    EXPECT_EQ(reference, strip);
}

TEST_F(ProcessingChainTest, insertTwo)
{
    processingChain.insertBlock(redSource);
    redSource = nullptr;
    processingChain.insertBlock(valueDoubler);
    valueDoubler = nullptr;

    auto reference = Processing::TRgbStrip(stripSize);
    reference[0] = { 20, 0, 0 };
    reference[1] = { 20, 0, 0 };
    reference[2] = { 20, 0, 0 };

    processingChain.execute(strip, Processing::TNoteToLightMap());
    EXPECT_EQ(reference, strip);
}

TEST_F(ProcessingChainTest, convertToJson)
{
    Json::array mockBlocksJson;
    for(unsigned int i = 0; i < 3; ++i)
    {
        TMockBlock* mockBlock = new TMockBlock;
        ASSERT_NE(nullptr, mockBlock);

        Json mockJson = createMockBlockJson(i);
        EXPECT_CALL(*mockBlock, convertToJson())
            .WillOnce(Return(mockJson));
        mockBlocksJson.push_back(mockJson);

        processingChain.insertBlock(mockBlock);
    }

    Json::object converted = processingChain.convertToJson().object_items();
    EXPECT_EQ(mockBlocksJson, converted["processingChain"].array_items());
    EXPECT_EQ("ProcessingChain", converted.at("objectType").string_value());
}

TEST_F(ProcessingChainTest, convertFromJson)
{
    std::vector<IProcessingBlock*> mockBlocks({greenSource, valueDoubler});

    // Processing chain takes over ownership of the mock block when our mock factory returns it.
    // Prevent that the fixture teardown deletes already deleted object
    greenSource = nullptr;
    valueDoubler = nullptr;

    Json::array mockBlocksJson;
    for(unsigned int i = 0; i < mockBlocks.size(); ++i)
    {
        Json mockJson = createMockBlockJson(i);
        mockBlocksJson.push_back(mockJson);
        EXPECT_CALL(processingBlockFactory, createProcessingBlock(mockJson))
            .WillOnce(Return(mockBlocks[i]));
    }
    Json::object j;
    j["processingChain"] = mockBlocksJson;
    processingChain.convertFromJson(Json(j));

    Processing::TRgbStrip reference(3);
    reference[0] = {0, 20, 0};
    reference[1] = {0, 20, 0};
    reference[2] = {0, 20, 0};
    Processing::TRgbStrip testStrip(3);
    processingChain.execute(testStrip, Processing::TNoteToLightMap());
    EXPECT_EQ(reference, testStrip);
}

TEST_F(ProcessingChainTest, activate)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        processingChain.insertBlock(block);
        EXPECT_CALL(*block, activate());
        block = nullptr;
    }

    processingChain.activate();
}

TEST_F(ProcessingChainTest, deactivate)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        processingChain.insertBlock(block);
        EXPECT_CALL(*block, deactivate());
        block = nullptr;
    }

    processingChain.deactivate();
}

TEST_F(ProcessingChainTest, activateOnInsert)
{
    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        EXPECT_CALL(*block, activate());
        processingChain.insertBlock(block);
        block = nullptr;
    }
}

TEST_F(ProcessingChainTest, deactivateOnInsert)
{
    processingChain.deactivate();

    for(int i = 0; i < 3; i++)
    {
        TMockBlock* block = new TMockBlock;
        EXPECT_CALL(*block, deactivate());
        processingChain.insertBlock(block);
        block = nullptr;
    }
}

class FakeAdditiveBlock
    : public IProcessingBlock
{
public:
    FakeAdditiveBlock(const Processing::TRgb& color) : color(color) {}

    void activate() override {}
    void deactivate() override {}
    void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap&) override
    {
        strip[0] = color;
    }

    Json convertToJson() const override { return Json(); };
    void convertFromJson(const Json& converted) override {};
    std::string getObjectType() const override { return ""; }

    Processing::TRgb color;
};

class FakeOverwritingBlock
    : public FakeAdditiveBlock
{
public:
    using FakeAdditiveBlock::FakeAdditiveBlock;
    Mode mode() const override
    {
        return Mode::overwriting;
    }
};

TEST_F(ProcessingChainTest, additive)
{
    using namespace Processing::ColorValue;
    processingChain.insertBlock(new FakeAdditiveBlock(red));
    processingChain.insertBlock(new FakeAdditiveBlock(green));

    processingChain.execute(strip, map);
    EXPECT_THAT(strip, ElementsAre(cyan, off, off));
}

TEST_F(ProcessingChainTest, overwriting)
{
    using namespace Processing::ColorValue;
    processingChain.insertBlock(new FakeAdditiveBlock(red));
    processingChain.insertBlock(new FakeOverwritingBlock(green));

    processingChain.execute(strip, map);
    EXPECT_THAT(strip, ElementsAre(green, off, off));
}

TEST_F(ProcessingChainTest, additiveAndOverwriting)
{
    using namespace Processing::ColorValue;
    processingChain.insertBlock(new FakeAdditiveBlock(red));
    processingChain.insertBlock(new FakeOverwritingBlock(green));
    processingChain.insertBlock(new FakeAdditiveBlock(blue));

    processingChain.execute(strip, map);
    EXPECT_THAT(strip, ElementsAre(yellow, off, off));
}

TEST_F(ProcessingChainTest, doNotAccumulateIntoNextCycle)
{
    processingChain.insertBlock(new FakeAdditiveBlock(Processing::TRgb{1, 0, 0}));

    processingChain.execute(strip, map);
    processingChain.execute(strip, map);
    EXPECT_EQ(strip[0], Processing::TRgb(1, 0, 0));
}
