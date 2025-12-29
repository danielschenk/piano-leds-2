#ifndef PROCESSING_MOCK_MOCKPROCESSINGCHAIN_H_
#define PROCESSING_MOCK_MOCKPROCESSINGCHAIN_H_

#include <gmock/gmock.h>

#include "../Interfaces/IProcessingChain.hpp"
#include "ProcessingBlock.hpp"

class MockProcessingChain : public IProcessingChain
{
  public:
    MOCK_METHOD2(insertBlock, void(ProcessingBlock* block, unsigned int index));
    MOCK_METHOD1(insertBlock, void(ProcessingBlock* block));
    MOCK_METHOD0(activate, void());
    MOCK_METHOD0(deactivate, void());
    MOCK_METHOD2(execute, void(processing::RgbStrip& strip, const ProcessingBlock::Input& input));
    MOCK_CONST_METHOD0(convertToJson, Json());
    MOCK_METHOD1(convertFromJson, void(const Json& converted));

  protected:
    MOCK_CONST_METHOD0(getObjectType, std::string());
};

#endif /* PROCESSING_MOCK_MOCKPROCESSINGCHAIN_H_ */
