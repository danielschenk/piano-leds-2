#include "ProcessingBlockFactory.hpp"

#include "IRgbFunctionFactory.hpp"
#include "Json11Helper.hpp"
#include "NoteVisualizer.hpp"
#include "Patch.hpp"
#include "ProcessingChain.hpp"
#include "SingleColorFill.hpp"

ProcessingBlockFactory::ProcessingBlockFactory(
    MidiInput& midiInput, const processing::IRgbFunctionFactory& rgbFunctionFactory,
    const MonotonicTime& time)
    : midiInput(midiInput), rgbFunctionFactory(rgbFunctionFactory), time(time)
{
}

ProcessingBlockFactory::~ProcessingBlockFactory() {}

ProcessingBlock* ProcessingBlockFactory::createProcessingBlock(const Json& converted) const
{
    ProcessingBlock* processingBlock = nullptr;

    Json11Helper helper(__PRETTY_FUNCTION__, converted);

    std::string objectType;
    if (helper.getItemIfPresent(JsonConvertible::objectTypeKey, objectType))
    {
        if (objectType == ProcessingBlock::typeNameSingleColorFill)
        {
            processingBlock = new SingleColorFill();
        }
        else if (objectType == ProcessingBlock::typeNameNoteVisualizer)
        {
            processingBlock = new NoteVisualizer(midiInput, rgbFunctionFactory, time);
        }
        else if (objectType == ProcessingBlock::typeNameProcessingChain)
        {
            // A processing chain needs the factory to construct its children
            processingBlock = new ProcessingChain(*this);
        }

        if (processingBlock != nullptr)
        {
            processingBlock->convertFromJson(converted);
        }
    }

    return processingBlock;
}

IPatch* ProcessingBlockFactory::createPatch() const
{
    // A patch needs the factory to construct its children
    return new Patch(*this);
}

IPatch* ProcessingBlockFactory::createPatch(const Json& converted) const
{
    IPatch* patch = createPatch();

    if (patch != nullptr)
    {
        patch->convertFromJson(converted);
    }

    return patch;
}

IProcessingChain* ProcessingBlockFactory::createProcessingChain() const
{
    // A patch needs the factory to construct its children
    return new ProcessingChain(*this);
}
