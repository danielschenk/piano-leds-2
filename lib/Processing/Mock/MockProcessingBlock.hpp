#ifndef PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_
#define PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_

#include <gmock/gmock.h>

#include "../ProcessingBlock.hpp"

class MockProcessingBlock : public ProcessingBlock
{
  public:
    MockProcessingBlock()
    {
        ON_CALL(*this, mode()).WillByDefault(::testing::Return(Mode::additive));
    }

    // ProcessingBlock implementation
    MOCK_METHOD0(activate, void());
    MOCK_METHOD0(deactivate, void());
    MOCK_METHOD2(execute, void(processing::RgbStrip& strip, const Input& input));
    MOCK_CONST_METHOD0(convertToJson, Json());
    MOCK_METHOD1(convertFromJson, void(const Json& converted));
    MOCK_CONST_METHOD0(mode, Mode());

  protected:
    MOCK_CONST_METHOD0(getObjectType, std::string());
};

#endif /* PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_ */
