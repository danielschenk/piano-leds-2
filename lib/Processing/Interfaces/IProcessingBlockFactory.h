#ifndef PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_
#define PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_

#include <json11.hpp>
// for convenience
using Json = json11::Json;

class IProcessingBlock;
class IPatch;
class IProcessingChain;

/**
 * Interface for processing block factories.
 */
class IProcessingBlockFactory
{
public:
    /**
     * Destructor.
     */
    virtual ~IProcessingBlockFactory()
    {
    }

    /**
     * Create a processing block from the given JSON.
     *
     * @param[in]   converted   JSON object containing the persistent properties.
     */
    virtual IProcessingBlock* createProcessingBlock(const Json& converted) const = 0;

    /**
     * Create a new patch.
     */
    virtual IPatch* createPatch() const = 0;

    /**
     * Create a patch from the given JSON.
     *
     * @param[in]   converted   JSON object containing the persistent properties.
     */
    virtual IPatch* createPatch(const Json& converted) const = 0;

    /**
     * Create a new processing chain.
     */
    virtual IProcessingChain* createProcessingChain() const = 0;
};


#endif /* PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_ */
