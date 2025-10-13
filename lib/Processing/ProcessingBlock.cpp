#include "ProcessingBlock.hpp"

void ProcessingBlock::activate() {}

void ProcessingBlock::deactivate() {}

std::map<std::string, processing::Modulatable*> ProcessingBlock::modulationSinks() const
{
    return std::map<std::string, processing::Modulatable*>();
}

ProcessingBlock::Mode ProcessingBlock::mode() const
{
    return Mode::additive;
}
