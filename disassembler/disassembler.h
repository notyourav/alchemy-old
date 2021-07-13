#pragma once

#include <span>
#include "disassembler/program.h"

class Disassembler {
public:

    Disassembler() { generate_masks(); }

    std::vector<PInst> disassemble(std::span<uint32_t> data);
    std::vector<PInstType> decodePInst(Instruction i);
};
