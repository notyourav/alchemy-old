#include "disassembler.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include "disassembler/program.h"
#include "elf.h"
#include "ut.h"

static void decode_operands(Instruction& i) {
    const char* enc = opcodes[i.opcode].encoding;
    int p = 0;                             // string offset
    int bit = 0;                           // opcode offset
    int len = 0;                           // current operand length
    OperandType type = OperandType::None;  // last evaluated operand

    while (enc[p] != '\0') {
        if (enc[p] == ':') {
            // separator
            p++;
            if (type != OperandType::None) {
                len++;
                bit++;
            }
        } else {
            if (type != OperandType::None) {
                uint32_t data = extract_bits(i.raw, 32 - bit, len);
                i.operands.emplace_back(32 - bit, len, data, type);
                len = 0;
                type = OperandType::None;
            }

            if (enc[p] == '-') {
                // reserved bit
                p++;
                bit++;
            } else {
                for (auto it = operand_type_name.begin(); it != operand_type_name.end(); ++it) {
                    if (strncmp(it->second, &enc[p], strlen(it->second)) == 0) {
                        type = it->first;
                        p += strlen(it->second);

                        // immediate is followed by its size (or r/s/lo/hi)
                        if (type == OperandType::Imm) {
                            if (enc[p] == 'r' || enc[p] == 's') {
                                p += 1;
                            } else if (strncmp(&enc[p], "lo", 2) == 0 ||
                                       strncmp(&enc[p], "hi", 2) == 0) {
                                p += 2;
                            } else {
                                char* end;
                                strtol(&enc[p], &end, 10);
                                p += (intptr_t)end - (intptr_t)&enc[p];
                            }
                            break;
                        }
                    }
                }
                if (type == OperandType::None) {
                    char buf[64];
                    strcpy(buf, &enc[p]);
                    printf("unhandled operand: %s\n", strtok(buf, ":"));
                    return;
                }
            }
        }
    }
    // capture last operand (fix this?)
    if (type != OperandType::None) {
        uint32_t data = extract_bits(i.raw, 32 - bit, len);
        i.operands.emplace_back(32 - bit, len, data, type);
    }
}

static Instruction decode_instr(uint32_t instr) {
    Instruction i = {.opcode = bad};

    for (int a = 0; a < 1115; ++a) {
        uint32_t h = opcodes[a].hex;
        if ((instr & op_masks[a]) == h) {
            i.opcode = (Opcode)a;
            i.raw = instr;
            decode_operands(i);
            break;
        }
    }

    return i;
}

// TODO: support variable length instructions.
std::vector<Instruction> Disassembler::disassemble(std::span<uint32_t> data) {
    std::vector<Instruction> result;

    for (auto& i : data) {
        Instruction instr = decode_instr(i);

        if (instr.opcode == bad) {
            // printf("bad opcode: %08x\n", i);
            // break;
        }

        result.emplace_back(instr);
    }

    printf("Done disassemble. instr_count %ld\n", result.size());
    return result;
}
