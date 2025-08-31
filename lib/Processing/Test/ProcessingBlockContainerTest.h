#ifndef PROCESSING_TEST_PROCESSINGBLOCKCONTAINERTEST_H_
#define PROCESSING_TEST_PROCESSINGBLOCKCONTAINERTEST_H_

#include <gtest/gtest.h>

#include <Mock/LoggingTest.h>

#include "../Mock/MockProcessingBlock.h"
#include "../Mock/MockProcessingBlockFactory.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Unused;
using ::testing::Return;

static void addRed(Processing::TRgbStrip& strip, Unused)
{
    for(auto& led : strip)
    {
        led.r = 10;
    }
}

static void addGreen(Processing::TRgbStrip& strip, Unused)
{
    for(auto& led : strip)
    {
        led.g = 10;
    }
}

static void doubleValue(Processing::TRgbStrip& strip, Unused)
{
    for(auto& led : strip)
    {
        led.r = std::min(0xff, led.r*2);
        led.g = std::min(0xff, led.g*2);
        led.b = std::min(0xff, led.b*2);
    }
}

/**
 * Base fixture for processing block container tests.
 */
class ProcessingBlockContainerTest
    : public LoggingTest
{
public:
    static constexpr unsigned int stripSize = 3;

    // Make processing block mocks nice, to prevent warnings about unexpected calls to execute().
    typedef NiceMock<MockProcessingBlock> TMockBlock;

    ProcessingBlockContainerTest()
        : LoggingTest()
        , redSource(new TMockBlock())
        , greenSource(new TMockBlock())
        , valueDoubler(new TMockBlock())
        , strip(stripSize)
        , processingBlockFactory()
    {
        ON_CALL(*redSource, execute(_, _))
            .WillByDefault(Invoke(addRed));
        ON_CALL(*greenSource, execute(_, _))
            .WillByDefault(Invoke(addGreen));
        ON_CALL(*valueDoubler, execute(_, _))
            .WillByDefault(Invoke(doubleValue));
        ON_CALL(*valueDoubler, mode())
            .WillByDefault(Return(IProcessingBlock::Mode::overwriting));
    }

    virtual ~ProcessingBlockContainerTest()
    {
        delete redSource;
        redSource = nullptr;
        delete greenSource;
        greenSource = nullptr;
        delete valueDoubler;
        valueDoubler = nullptr;
    }

    Json createMockBlockJson(unsigned int id)
    {
        Json::object j;
        j["id"] = static_cast<int>(id);

        return Json(j);
    }

    // These have to be manually allocated, as the container under test takes ownership and will try to delete it's children.
    TMockBlock* redSource;
    TMockBlock* greenSource;
    TMockBlock* valueDoubler;

    Processing::TRgbStrip strip;

    MockProcessingBlockFactory processingBlockFactory;
};


#endif /* PROCESSING_TEST_PROCESSINGBLOCKCONTAINERTEST_H_ */
