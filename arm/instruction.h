#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include "op_table.h"

enum Register {
    r0 = 0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    r16,
    r17,
    r18,
    r19,
    r20,
    r21,
    r22,
    r23,
    r24,
    r25,
    r26,
    r27,
    r28,
    r29_fr,
    r30_lr,
    sp_zr
};

enum OperandType {
    None,
    Imm,
    Rn,
    Rd,
    Rt2,
    Rt,
    Rs,
    Rm,
    cond,
    shift,
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
    NV = 15
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

extern std::map<Register, const char*> register_name;
extern std::map<ConditionCode, const char*> condition_code_name;
extern std::map<OperandType, const char*> operand_type_name;

extern uint32_t* op_masks;

void print_instr(Instruction i);
void generate_masks();

const char* operandToString(OperandType t);
