#include <Json11Helper.h>

#include "Patch.h"
#include "IProcessingBlockFactory.h"

Patch::Patch(const IProcessingBlockFactory& processingBlockFactory)
    : processingChain(processingBlockFactory.createProcessingChain())
    , processingBlockFactory(processingBlockFactory)
{
}

Patch::~Patch()
{
    delete processingChain;
}

IProcessingChain& Patch::getProcessingChain() const
{
    return *processingChain;
}

Json Patch::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object converted;
    converted[JsonConvertible::objectTypeKey] = getObjectType();

    // Add items specific for Patch
    converted[hasBankAndProgramJsonKey] = bankAndProgramSet;
    converted[bankJsonKey] = bank;
    converted[programJsonKey] = program;
    converted[nameJsonKey] = name;

    // Add processing chain
    converted[processingChainJsonKey] = processingChain->convertToJson();

    return converted;
}

void Patch::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);

    // Get items specific for Patch
    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(hasBankAndProgramJsonKey, bankAndProgramSet);
    helper.getItemIfPresent(programJsonKey, program);
    helper.getItemIfPresent(bankJsonKey, bank);
    helper.getItemIfPresent(nameJsonKey, name);

    // Get processing chain
    Json::object convertedProcessingChain;
    if(helper.getItemIfPresent(processingChainJsonKey, convertedProcessingChain))
    {
        processingChain->convertFromJson(convertedProcessingChain);
    }
    else
    {
        // Reset to default.
        delete processingChain;
        processingChain = processingBlockFactory.createProcessingChain();
    }
}

std::string Patch::getObjectType() const
{
    return typeName;
}

void Patch::activate()
{
    processingChain->activate();
}

void Patch::deactivate()
{
    processingChain->deactivate();
}

void Patch::execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap)
{
    processingChain->execute(strip, noteToLightMap);
}

uint8_t Patch::getBank() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return bank;
}

void Patch::setBank(uint8_t bank)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->bank = bank;
}

bool Patch::hasBankAndProgram() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return bankAndProgramSet;
}

uint8_t Patch::getProgram() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return program;
}

void Patch::clearBankAndProgram()
{
    std::lock_guard<std::mutex> lock(mutex);

    bankAndProgramSet = false;
}

void Patch::setProgram(uint8_t program)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->program = program;
    bankAndProgramSet = true;
}

std::string Patch::getName() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return name;
}

void Patch::setName(const std::string name)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->name = name;
}
