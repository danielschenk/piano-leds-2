#include "ProcessingBlock.hpp"

void ProcessingBlock::activate() {}

void ProcessingBlock::deactivate() {}

ProcessingBlock::Mode ProcessingBlock::mode() const
{
    return Mode::additive;
}
