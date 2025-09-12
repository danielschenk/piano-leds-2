#ifndef PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_
#define PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_

#include <json11.hpp>
// for convenience
using Json = json11::Json;

class ProcessingBlock;
class IPatch;
class IProcessingChain;

class IProcessingBlockFactory
{
  public:
    virtual ~IProcessingBlockFactory() = default;

    virtual ProcessingBlock* createProcessingBlock(const Json& converted) const = 0;
    virtual IPatch* createPatch() const = 0;
    virtual IPatch* createPatch(const Json& converted) const = 0;
    virtual IProcessingChain* createProcessingChain() const = 0;
};

#endif /* PROCESSING_INTERFACES_IPROCESSINGBLOCKFACTORY_H_ */
