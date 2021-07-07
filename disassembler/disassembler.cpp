#include "disassembler.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "disassembler/program.h"
#include "ut.h"

static uint32_t swap_endian(uint32_t instr);
static void text_get_info(const char* path);

static long text_offset = 0;
static size_t text_size = 0;

#define INSTR_LENGTH 4

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

                        // immediate is followed by its size
                        if (type == OperandType::Imm) {
                            char* end;
                            strtol(&enc[p], &end, 10);
                            p += (intptr_t)end - (intptr_t)&enc[p];
                        }
                        break;
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
            print_instr(i);
            break;
        }
    }

    return i;
}

// TODO: support variable length instructions.
const Program& Disassembler::init(std::span<uint32_t> data) {
    for (auto& i : data) {
        Instruction instr = decode_instr(i);

        if (instr.opcode == bad) {
            // printf("bad opcode: %08x\n", i);
            // break;
        }

        mProgram.instructions.emplace_back(instr);
    }

    printf("Done disassemble. instr_count %ld\n", mProgram.instructions.size());
    return mProgram;
}

const Program& Disassembler::init(const char* path) {
    text_get_info(path);
    printf("Text section offset: 0x%lx\n", text_offset);
    printf("Text section size: 0x%lx\n", text_size);

    FILE* f = fopen(path, "r");
    uint32_t* dump = (uint32_t*)malloc(text_size);
    fseek(f, text_offset, SEEK_SET);
    fread(dump, text_size, 1, f);
    fclose(f);

    init({dump, text_size});
    free(dump);
    return mProgram;
}

static uint32_t __attribute__((unused)) swap_endian(uint32_t instr) {
    uint32_t result = 0;
    result = ((instr >> 24) & 0xFF) | ((instr << 8) & 0xFF0000) | ((instr >> 8) & 0xFF00) |
             ((instr << 24) & 0xFF000000);
    return result;
}

#define DUMP_CMD "objdump -h %s"
#define DUMP_SENTINEL "__text"

/*
 * Use objdump to find the offset of the text section.
 * TODO: Parse executable manually, add support for ELF
 *  This is not portable.
 */
static void text_get_info(const char* path) {
    char buffer[64];
    char cmd[64];
    FILE* pipe = NULL;

    sprintf(cmd, DUMP_CMD, path);
    pipe = popen(cmd, "r");

    while (fgets(buffer, sizeof buffer, pipe) != NULL) {
        if (strstr(buffer, DUMP_SENTINEL) != NULL) {
            char* end = NULL;
            char size[64];
            char vma[64];

            sscanf(buffer, "%*s %*s %s %s", size, vma);
            text_offset = strtol(vma, &end, 16) - 0x100000000;
            text_size = strtol(size, &end, 16);
        }
    }
    assert(text_offset != 0);
    assert(text_size != 0);
}
