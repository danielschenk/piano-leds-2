#include <Json11Helper.h>

#include "ProcessingBlockFactory.h"
#include "NoteRgbSource.h"
#include "EqualRangeRgbSource.h"
#include "ProcessingChain.h"
#include "Patch.h"

ProcessingBlockFactory::ProcessingBlockFactory(IMidiInput& midiInput,
                                               const IRgbFunctionFactory& rgbFunctionFactory,
                                               const ITime& time)
    : midiInput(midiInput)
    , rgbFunctionFactory(rgbFunctionFactory)
    , time(time)
{
}

ProcessingBlockFactory::~ProcessingBlockFactory()
{
}

IProcessingBlock* ProcessingBlockFactory::createProcessingBlock(const Json& converted) const
{
    IProcessingBlock* processingBlock = nullptr;

    Json11Helper helper(__PRETTY_FUNCTION__, converted);

    std::string objectType;
    if(helper.getItemIfPresent(JsonConvertible::objectTypeKey, objectType))
    {
        if(objectType == IProcessingBlock::typeNameEqualRangeRgbSource)
        {
            processingBlock = new EqualRangeRgbSource();
        }
        else if(objectType == IProcessingBlock::typeNameNoteRgbSource)
        {
            processingBlock = new NoteRgbSource(midiInput, rgbFunctionFactory, time);
        }
        else if(objectType == IProcessingBlock::typeNameProcessingChain)
        {
            // A processing chain needs the factory to construct its children
            processingBlock = new ProcessingChain(*this);
        }

        if(processingBlock != nullptr)
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

    if(patch != nullptr)
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
