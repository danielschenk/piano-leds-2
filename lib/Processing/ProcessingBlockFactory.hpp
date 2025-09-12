#ifndef PROCESSING_PROCESSINGBLOCKFACTORY_H_
#define PROCESSING_PROCESSINGBLOCKFACTORY_H_

#include "IProcessingBlockFactory.hpp"

class MidiInput;
class IRgbFunctionFactory;
class MonotonicTime;

/**
 * Factory for processing blocks.
 */
class ProcessingBlockFactory : public IProcessingBlockFactory
{
  public:
    /**
     * Constructor.
     */
    ProcessingBlockFactory(MidiInput& midiInput, const IRgbFunctionFactory& rgbFunctionFactory,
                           const MonotonicTime& time);

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
    MidiInput& midiInput;

    /** Reference to the RGB function factory to pass to new blocks. */
    const IRgbFunctionFactory& rgbFunctionFactory;

    /** Time provider to pass to new blocks. */
    const MonotonicTime& time;
};

#endif /* PROCESSING_PROCESSINGBLOCKFACTORY_H_ */
