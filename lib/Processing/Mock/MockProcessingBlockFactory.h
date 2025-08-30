#ifndef PROCESSING_MOCK_MOCKPROCESSINGBLOCKFACTORY_H_
#define PROCESSING_MOCK_MOCKPROCESSINGBLOCKFACTORY_H_

#include <gmock/gmock.h>

#include "../Interfaces/IProcessingBlockFactory.h"

class MockProcessingBlockFactory
    : public IProcessingBlockFactory
{
public:
    MOCK_CONST_METHOD1(createProcessingBlock, IProcessingBlock*(const Json& converted));
    MOCK_CONST_METHOD0(createPatch, IPatch*());
    MOCK_CONST_METHOD1(createPatch, IPatch*(const Json& converted));
    MOCK_CONST_METHOD0(createProcessingChain, IProcessingChain*());
};


#endif /* PROCESSING_MOCK_MOCKPROCESSINGBLOCKFACTORY_H_ */
