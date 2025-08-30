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
    /**
     * Constructor.
     */
    ProcessingChain(const IProcessingBlockFactory& processingBlockFactory);

    /**
     * Destructor.
     */
    virtual ~ProcessingChain();

    // Prevent implicit constructor, copy constructor and assignment operator.
    ProcessingChain() = delete;
    ProcessingChain(const ProcessingChain&) = delete;
    ProcessingChain& operator=(const ProcessingChain&) = delete;

    // IProcessingChain implementation
    virtual void activate();
    virtual void deactivate();
    virtual void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap);
    virtual void insertBlock(IProcessingBlock* block, unsigned int index);
    virtual void insertBlock(IProcessingBlock* block);
    virtual Json convertToJson() const;
    virtual void convertFromJson(const Json& converted);

protected:
    // IProcessingBlock implementation
    virtual std::string getObjectType() const;

private:
    static constexpr const char* processingChainJsonKey = "processingChain";

    /** Mutex to protect the members. */
    mutable std::mutex mutex;

    /** Reference to the processing block factory. */
    const IProcessingBlockFactory& processingBlockFactory;

    /** Whether all blocks in the chain are active or not. */
    bool active;

    /** The processing chain. Using a vector for optimal traversal. */
    std::vector<IProcessingBlock*> processingChain;

    void deleteProcessingBlocks();

    /** Activates/deactivates every block in the chain, based on whether we're active or not. */
    void updateAllBlockStates();
};

#endif /* PROCESSING_PROCESSINGCHAIN_H_ */
