#ifndef PROCESSING_MOCK_MOCKPATCH_H_
#define PROCESSING_MOCK_MOCKPATCH_H_

#include <gmock/gmock.h>

#include "../Interfaces/IPatch.hpp"

class MockPatch : public IPatch
{
  public:
    MOCK_CONST_METHOD0(getProcessingChain, IProcessingChain&());
    MOCK_METHOD0(activate, void());
    MOCK_METHOD0(deactivate, void());
    MOCK_METHOD2(execute, void(processing::RgbStrip& strip,
                               const processing::TNoteToLightMap& noteToLightMap));
    MOCK_CONST_METHOD0(hasBankAndProgram, bool());
    MOCK_CONST_METHOD0(getBank, uint8_t());
    MOCK_METHOD1(setBank, void(uint8_t bank));
    MOCK_CONST_METHOD0(getProgram, uint8_t());
    MOCK_METHOD1(setProgram, void(uint8_t program));
    MOCK_METHOD0(clearBankAndProgram, void());
    MOCK_CONST_METHOD0(getName, std::string());
    MOCK_METHOD1(setName, void(std::string name));
    MOCK_CONST_METHOD0(convertToJson, Json());
    MOCK_METHOD1(convertFromJson, void(const Json& converted));

  protected:
    MOCK_CONST_METHOD0(getObjectType, std::string());
};

#endif /* PROCESSING_MOCK_MOCKPATCH_H_ */
