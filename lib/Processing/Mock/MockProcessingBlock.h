#ifndef PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_
#define PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_

#include <gmock/gmock.h>

#include "../Interfaces/IProcessingBlock.h"

class MockProcessingBlock
    : public IProcessingBlock
{
public:
    // IProcessingBlock implementation
    MOCK_METHOD0(activate, void());
    MOCK_METHOD0(deactivate, void());
    MOCK_METHOD2(execute, void(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap));
    MOCK_CONST_METHOD0(convertToJson, Json());
    MOCK_METHOD1(convertFromJson, void(const Json& converted));

protected:
    MOCK_CONST_METHOD0(getObjectType, std::string());
};

#endif /* PROCESSING_MOCK_MOCKPROCESSINGBLOCK_H_ */
