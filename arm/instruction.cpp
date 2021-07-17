#include "instruction.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "ut.h"

uint32_t* op_masks;
std::map<Register, const char*> register_name = {
    {r0, "r0"},      {r1, "r1"},   {r2, "r2"},   {r3, "r3"},   {r4, "r4"},   {r5, "r5"},
    {r6, "r6"},      {r7, "r7"},   {r8, "r8"},   {r9, "r9"},   {r10, "r10"}, {r11, "r11"},
    {r12, "r12"},    {r13, "r13"}, {r14, "r14"}, {r15, "r15"}, {r16, "r16"}, {r17, "r17"},
    {r18, "r18"},    {r19, "r19"}, {r20, "r20"}, {r21, "r21"}, {r22, "r22"}, {r23, "r23"},
    {r24, "r24"},    {r25, "r25"}, {r26, "r26"}, {r27, "r27"}, {r28, "r28"}, {r29_fr, "r29"},
    {r30_lr, "r30"}, {sp_zr, "sp"}};
std::map<OperandType, const char*> operand_type_name = {
    {None, "None"}, {Imm, "imm"}, {Rn, "Rn"}, {Rd, "Rd"},    {Rt2, "Rt2"},
    {Rt, "Rt"},     {Rs, "Rs"},   {Rm, "Rm"}, {cond, "cond"}, {shift, "shift"}};
std::map<ConditionCode, const char*> condition_code_name = {
    {EQ, "eq"}, {NE, "ne"}, {CS, "cs"}, {CC, "cc"}, {MI, "mi"}, {PL, "pl"}, {VS, "vs"}, {VC, "vc"},
    {HI, "hi"}, {LS, "ls"}, {GE, "ge"}, {LT, "lt"}, {GT, "gt"}, {LE, "le"}, {AL, "al"}, {NV, "nv"}};

void print_instr(Instruction i) {
    printf("%s", opcodes[i.opcode].mnemonic);

    for (auto& o : i.operands) {
        switch (o.type) {
        case Rn:
        case Rd:
        case Rt:
        case Rt2:
        case Rs:
        case Rm:
            printf(" %s", register_name[static_cast<Register>(o.data)]);
            break;
        case Imm:
            printf(" #%u", o.data);
            break;
        case cond:
            printf(" %s", condition_code_name[static_cast<ConditionCode>(o.data)]);
            break;
        default:
            break;
        }
    }

    //    for (auto opr : i.operands) {
    //        //printf("\n%s", operandToString(opr.type));
    //        //printf("(len = %d, off = %d, data = %d)", opr.length, opr.offset, opr.data);
    //        switch(opr.type) {
    //            case OperandType::cond:
    //                printf(".%s", condToString((ConditionCode)opr.data));
    //            case None:
    //                break;
    //            case Imm:
    //                printf(" #%u", opr.data);
    //                break;
    //            case Rn:
    //            case Rd:
    //            case Rt2:
    //            case Rt:
    //                printf(" x%u", opr.data);
    //                break;
    //        }
    //    }

    printf("\n");
}

// generate a mask from instruction specs
void generate_masks() {
    int op_count = sizeof(opcodes) / sizeof(OpcodeListing);
    op_masks = (uint32_t*)malloc(op_count * sizeof(uint32_t));
    for (int i = 0; i < op_count; ++i) {
        const OpcodeListing* op = &opcodes[i];
        uint32_t mask = 0;
        const char* split = op->encoding;
        uint32_t bit = 0;

        while (split != nullptr) {
            if (split[0] == '-') {
                mask |= (1 << (31 - bit));
            }
            split = strchr(++split, ':');
            if (split != nullptr && split[1] != ':')
                ++split;
            bit++;
        }
        op_masks[i] = mask;
    }
}
