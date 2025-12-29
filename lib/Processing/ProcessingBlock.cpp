#include "ProcessingBlock.hpp"

void ProcessingBlock::activate() {}

void ProcessingBlock::deactivate() {}

bool ProcessingBlock::valid(const Input& input)
{
    return true;
}

ProcessingBlock::Mode ProcessingBlock::mode() const
{
    return Mode::additive;
}
