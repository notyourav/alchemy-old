#pragma once

#include <span>
#include "arm/instruction.h"
#include "disassembler/program.h"

class Disassembler {
public:
    Disassembler() { generate_masks(); }

    const Program& init(const char* path);
    const Program& init(std::span<uint32_t> instructions);

private:
    Program mProgram;
};
