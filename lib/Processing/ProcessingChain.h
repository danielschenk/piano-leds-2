#ifndef PROCESSING_PROCESSINGCHAIN_H_
#define PROCESSING_PROCESSINGCHAIN_H_

#include <mutex>
#include <list>

#include "IProcessingChain.h"

class IProcessingBlockFactory;

/**
 * A processing chain which can hold multiple processing blocks connected in series.
 */
class ProcessingChain
    : public IProcessingChain
{
public:
    explicit ProcessingChain(const IProcessingBlockFactory& processingBlockFactory);
    ~ProcessingChain() override;

    ProcessingChain(const ProcessingChain&) = delete;
    ProcessingChain& operator=(const ProcessingChain&) = delete;

    // IProcessingChain implementation
    void activate() override;
    void deactivate() override;
    void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap) override;
    void insertBlock(ProcessingBlock* block, unsigned int index) override;
    void insertBlock(ProcessingBlock* block) override;
    Json convertToJson() const override;
    void convertFromJson(const Json& converted) override;

protected:
    // IProcessingBlock implementation
    std::string getObjectType() const override;

private:
    static constexpr const char* processingChainJsonKey = "processingChain";

    mutable std::mutex mutex;

    const IProcessingBlockFactory& processingBlockFactory;
    bool active{false};
    std::vector<ProcessingBlock*> processingChain;
    Processing::TRgbStrip intermediateStrip;

    void deleteProcessingBlocks();

    /** Activates/deactivates every block in the chain, based on whether we're active or not. */
    void updateAllBlockStates();
};

#endif /* PROCESSING_PROCESSINGCHAIN_H_ */
