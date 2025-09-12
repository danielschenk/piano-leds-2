#include "ProcessingChain.h"

#include <Json11Helper.h>
#include <Logging.h>

#include <algorithm>

#include "IProcessingBlockFactory.h"
#include "ProcessingBlock.h"

#define LOGGING_COMPONENT "ProcessingChain"

ProcessingChain::ProcessingChain(const IProcessingBlockFactory& processingBlockFactory)
    : processingBlockFactory(processingBlockFactory)
{
}

ProcessingChain::~ProcessingChain()
{
    deleteProcessingBlocks();
}

void ProcessingChain::insertBlock(ProcessingBlock* block, unsigned int index)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (index > processingChain.size())
    {
        index = processingChain.size();
    }

    processingChain.insert(processingChain.begin() + index, block);
    active ? block->activate() : block->deactivate();
}

void ProcessingChain::insertBlock(ProcessingBlock* block)
{
    std::lock_guard<std::mutex> lock(mutex);

    processingChain.insert(processingChain.end(), block);
    active ? block->activate() : block->deactivate();
}

Json ProcessingChain::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object converted;
    converted[ProcessingBlock::objectTypeKey] = getObjectType();

    Json::array convertedChain;
    for (auto processingBlock : processingChain)
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
    if (helper.getItemIfPresent(processingChainJsonKey, convertedChain))
    {
        for (auto convertedBlock : convertedChain)
        {
            processingChain.push_back(processingBlockFactory.createProcessingBlock(convertedBlock));
        }
    }
    else
    {
        LOG_ERROR(
            "convertFromJson: JSON does not contain list of processing blocks. Chain will stay "
            "empty.");
    }

    updateAllBlockStates();
}

std::string ProcessingChain::getObjectType() const
{
    return ProcessingBlock::typeNameProcessingChain;
}

void ProcessingChain::activate()
{
    std::lock_guard<std::mutex> lock(mutex);

    for (auto processingBlock : processingChain)
    {
        processingBlock->activate();
    }

    active = true;
}

void ProcessingChain::deactivate()
{
    std::lock_guard<std::mutex> lock(mutex);

    for (auto processingBlock : processingChain)
    {
        processingBlock->deactivate();
    }

    active = false;
}

void ProcessingChain::execute(Processing::TRgbStrip& strip,
                              const Processing::TNoteToLightMap& noteToLightMap)
{
    std::lock_guard<std::mutex> lock(mutex);

    std::fill(strip.begin(), strip.end(), Processing::ColorValue::off);

    for (auto processingBlock : processingChain)
    {
        auto mode = processingBlock->mode();
        if (mode == ProcessingBlock::Mode::additive)
        {
            intermediateStrip.resize(strip.size());
            std::fill(intermediateStrip.begin(), intermediateStrip.end(),
                      Processing::ColorValue::off);

            processingBlock->execute(intermediateStrip, noteToLightMap);

            std::transform(strip.begin(), strip.end(), intermediateStrip.begin(), strip.begin(),
                           std::plus<Processing::TRgb>());
        }
        else if (mode == ProcessingBlock::Mode::overwriting)
        {
            processingBlock->execute(strip, noteToLightMap);
        }
    }
}

void ProcessingChain::deleteProcessingBlocks()
{
    for (auto processingBlock : processingChain)
    {
        delete processingBlock;
    }
    processingChain.clear();
}

void ProcessingChain::updateAllBlockStates()
{
    if (active)
    {
        for (auto processingBlock : processingChain)
        {
            processingBlock->activate();
        }
    }
    else
    {
        for (auto processingBlock : processingChain)
        {
            processingBlock->deactivate();
        }
    }
}
