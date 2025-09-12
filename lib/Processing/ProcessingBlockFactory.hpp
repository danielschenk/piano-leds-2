#ifndef PROCESSING_PROCESSINGBLOCKFACTORY_H_
#define PROCESSING_PROCESSINGBLOCKFACTORY_H_

#include "IProcessingBlockFactory.hpp"
#include "ProcessingTypes.hpp"

class IMidiInput;
class IRgbFunctionFactory;
class ITime;

/**
 * Factory for processing blocks.
 */
class ProcessingBlockFactory : public IProcessingBlockFactory
{
  public:
    /**
     * Constructor.
     */
    ProcessingBlockFactory(IMidiInput& midiInput, const IRgbFunctionFactory& rgbFunctionFactory,
                           const ITime& time);

    // Prevent implicit constructors and assignment operator
    ProcessingBlockFactory() = delete;
    ProcessingBlockFactory(const ProcessingBlockFactory&) = delete;
    ProcessingBlockFactory& operator=(const ProcessingBlockFactory&) = delete;

    /**
     * Destructor.
     */
    virtual ~ProcessingBlockFactory();

    // IProcessingBlockFactory implementation
    virtual ProcessingBlock* createProcessingBlock(const Json& converted) const;
    virtual IPatch* createPatch() const;
    virtual IPatch* createPatch(const Json& converted) const;
    virtual IProcessingChain* createProcessingChain() const;

  private:
    /** Reference to the MIDI input to pass to new blocks. */
    IMidiInput& midiInput;

    /** Reference to the RGB function factory to pass to new blocks. */
    const IRgbFunctionFactory& rgbFunctionFactory;

    /** Time provider to pass to new blocks. */
    const ITime& time;
};

#endif /* PROCESSING_PROCESSINGBLOCKFACTORY_H_ */
