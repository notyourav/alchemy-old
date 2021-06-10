#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "instruction.h"
#include "ut.h"

uint32_t* op_masks;

const char* condToString(ConditionCode i) {
    switch (i) {
        default:
        case ConditionCode::EQ:
            return "EQ";
        case ConditionCode::NE:
            return "NE";
        case ConditionCode::CS:
            return "CS";
        case ConditionCode::CC:
            return "CC";
        case ConditionCode::MI:
            return "MI";
        case ConditionCode::PL:
            return "PL";
        case ConditionCode::VS:
            return "VS";
        case ConditionCode::VC:
            return "VC";
        case ConditionCode::HI:
            return "HI";
        case ConditionCode::LS:
            return "LS";
        case ConditionCode::GE:
            return "GE";
        case ConditionCode::LT:
            return "LT";
        case ConditionCode::GT:
            return "GT";
        case ConditionCode::LE:
            return "LE";
        case ConditionCode::AL:
            return "AL";
        case ConditionCode::NV:
            return "NV";
    }
}

const char* operandToString(OperandType t) {
    switch (t) {
        default:
        case OperandType::None:
            return "None";
        case OperandType::Imm:
            return "Imm";
        case OperandType::Rd:
            return "Rd";
        case OperandType::Rn:
            return "Rn";
        case OperandType::Rt:
            return "Rt";
        case OperandType::Rt2:
            return "Rt2";
        case OperandType::cond:
            return "cond";
    }
}

void print_instr(Instruction i) {
    printf("%s", opcodes[i.opcode].mnemonic);

    for (auto opr : i.operands) {
        //printf("\n%s", operandToString(opr.type));
        //printf("(len = %d, off = %d, data = %d)", opr.length, opr.offset, opr.data);
        switch(opr.type) {
            case OperandType::cond:
                printf(".%s", condToString((ConditionCode)opr.data));
            case None:
                break;
            case Imm:
                printf(" #%u", opr.data);
                break;
            case Rn:
            case Rd:
            case Rt2:
            case Rt:
                printf(" x%u", opr.data);
                break;
        }
    }

    printf("\n");
}

// generate a mask from instruction specs
void generate_masks() {
    int op_count = sizeof(opcodes) / sizeof(OpcodeListing);
    op_masks = (uint32_t *)malloc(op_count * sizeof(uint32_t));
    for (int i = 0; i < op_count; ++i) {
        const OpcodeListing* op = &opcodes[i];
        uint32_t mask = 0;
        const char* split = op->encoding;
        uint32_t bit = 0;

        while (split != NULL) {
            if (split[0] == '-') {
                mask |= (1 << (31 - bit));
            }
            split = strchr(++split, ':');
            if (split != NULL && split[1] != ':') ++split;
            bit++;
        }
        op_masks[i] = mask;
    }
}
