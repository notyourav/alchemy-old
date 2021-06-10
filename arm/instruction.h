#ifndef ALCHEMY_INSTRUCTION_H
#define ALCHEMY_INSTRUCTION_H

#include <cstdint>
#include <vector>
#include "op_table.h"

enum OperandType {
    None,
    Imm,
    Rn,
    Rd,
    Rt2,
    Rt,
    cond
};

enum Register {
    x29 = 2,
    sp = 4,
};

enum ConditionCode {
    EQ = 0,
    NE = 1,
    CS = 2,
    CC = 3,
    MI = 4,
    PL = 5,
    VS = 6,
    VC = 7,
    HI = 8,
    LS = 9,
    GE = 10,
    LT = 11,
    GT = 12,
    LE = 13,
    AL = 14,
    NV = 15,
};

struct Operand {
    Operand(uint32_t offset, uint32_t length, uint32_t data, OperandType type) {
        this->offset = offset;
        this->length = length;
        this->data = data;
        this->type = type;
    }

    uint32_t offset;
    uint32_t length;
    uint32_t data;
    OperandType type;
};

struct Instruction {
    Opcode opcode;
    uint32_t raw;
    std::vector<Operand> operands;
};

extern uint32_t* op_masks;

void print_instr(Instruction i);
void generate_masks();

const char* operandToString(OperandType t);

#endif //ALCHEMY_INSTRUCTION_H
