#pragma once

#include "arm/instruction.h"
#include <vector>
#include <span>

class Disassembler;

struct Program {
    struct TextInfo {
        long offset;
        size_t size;
    };

    Program(const char* path, Disassembler* dis);

    TextInfo text_info;
    std::vector<Instruction> instructions;
};
