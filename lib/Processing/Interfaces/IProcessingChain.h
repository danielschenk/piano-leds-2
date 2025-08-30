#ifndef PROCESSING_INTERFACES_IPROCESSINGCHAIN_H_
#define PROCESSING_INTERFACES_IPROCESSINGCHAIN_H_

#include "IProcessingBlock.h"

/**
 * Interface for processing chains.
 */
class IProcessingChain
    : public IProcessingBlock
{
public:
    virtual ~IProcessingChain() = default;

    /**
     * Insert a processing block.
     *
     * @param[in]   block   Pointer to the processing block.
     * @param[in]   index   Index in the chain at which the block should be inserted.
     */
    virtual void insertBlock(IProcessingBlock* block, unsigned int index) = 0;

    /**
     * Insert a processing block at the end.
     *
     * @param[in]   block   Pointer to the processing block.
     */
    virtual void insertBlock(IProcessingBlock* block) = 0;
};



#endif /* PROCESSING_INTERFACES_IPROCESSINGCHAIN_H_ */
