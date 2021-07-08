#pragma once

#include "arm/instruction.h"
#include <vector>
#include <span>

struct Program {
    std::vector<Instruction> instructions;
    long text_offset;
    size_t text_size;
};
