#pragma once

#include <span>
#include "arm/instruction.h"
#include "disassembler/program.h"

class Disassembler {
public:

    Disassembler() { generate_masks(); }

    std::vector<Instruction> disassemble(std::span<uint32_t> data);
};
