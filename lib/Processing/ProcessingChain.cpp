#include <Logging.h>
#include <Json11Helper.h>

#include "ProcessingChain.h"

#include "IProcessingBlock.h"
#include "IProcessingBlockFactory.h"

#define LOGGING_COMPONENT "ProcessingChain"

ProcessingChain::ProcessingChain(const IProcessingBlockFactory& processingBlockFactory)
    : mutex()
    , processingBlockFactory(processingBlockFactory)
    , active()
    , processingChain()
{
}

ProcessingChain::~ProcessingChain()
{
    deleteProcessingBlocks();
}

void ProcessingChain::insertBlock(IProcessingBlock* block, unsigned int index)
{
    std::lock_guard<std::mutex> lock(mutex);

    if(index > processingChain.size())
    {
        index = processingChain.size();
    }

    processingChain.insert(processingChain.begin() + index, block);
    active ? block->activate() : block->deactivate();
}

void ProcessingChain::insertBlock(IProcessingBlock* block)
{
    std::lock_guard<std::mutex> lock(mutex);

    processingChain.insert(processingChain.end(), block);
    active ? block->activate() : block->deactivate();
}

Json ProcessingChain::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object converted;
    converted[IProcessingBlock::objectTypeKey] = getObjectType();

    Json::array convertedChain;
    for(auto processingBlock : processingChain)
    {
        convertedChain.push_back(processingBlock->convertToJson());
    }
    converted[processingChainJsonKey] = convertedChain;

    return Json(converted);
}

void ProcessingChain::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);

    deleteProcessingBlocks();

    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    Json::array convertedChain;
    if(helper.getItemIfPresent(processingChainJsonKey, convertedChain))
    {
        for(auto convertedBlock : convertedChain)
        {
            processingChain.push_back(processingBlockFactory.createProcessingBlock(convertedBlock));
        }
    }
    else
    {
        LOG_ERROR("convertFromJson: JSON does not contain list of processing blocks. Chain will stay empty.");
    }

    updateAllBlockStates();
}

std::string ProcessingChain::getObjectType() const
{
    return IProcessingBlock::typeNameProcessingChain;
}

void ProcessingChain::activate()
{
    std::lock_guard<std::mutex> lock(mutex);

    for(auto processingBlock : processingChain)
    {
        processingBlock->activate();
    }

    active = true;
}

void ProcessingChain::deactivate()
{
    std::lock_guard<std::mutex> lock(mutex);

    for(auto processingBlock : processingChain)
    {
        processingBlock->deactivate();
    }

    active = false;
}

void ProcessingChain::execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap)
{
    std::lock_guard<std::mutex> lock(mutex);

    // Start clean
    for(auto& color : strip)
    {
        color.r = 0;
        color.g = 0;
        color.b = 0;
    }

    for(auto processingBlock : processingChain)
    {
        processingBlock->execute(strip, noteToLightMap);
    }
}

void ProcessingChain::deleteProcessingBlocks()
{
    for(auto processingBlock : processingChain)
    {
        delete processingBlock;
    }
    processingChain.clear();
}

void ProcessingChain::updateAllBlockStates()
{
    if(active)
    {
        for(auto processingBlock : processingChain)
        {
            processingBlock->activate();
        }
    }
    else
    {
        for(auto processingBlock : processingChain)
        {
            processingBlock->deactivate();
        }
    }
}
