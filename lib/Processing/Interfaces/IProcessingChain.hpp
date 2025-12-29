#ifndef PROCESSING_INTERFACES_IPROCESSINGCHAIN_H_
#define PROCESSING_INTERFACES_IPROCESSINGCHAIN_H_

#include "JsonConvertible.hpp"
#include "ProcessingBlock.hpp"
#include "ProcessingTypes.hpp"

class IProcessingChain : public JsonConvertible
{
  public:
    virtual ~IProcessingChain() = default;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void execute(processing::RgbStrip& strip, const ProcessingBlock::Input& input) = 0;

    virtual void insertBlock(ProcessingBlock* block, unsigned int index) = 0;
    virtual void insertBlock(ProcessingBlock* block) = 0;
};

#endif /* PROCESSING_INTERFACES_IPROCESSINGCHAIN_H_ */
