#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "disassembler.h"
#include "arm/instruction.h"
#include "ut.h"

static void disassemble(void);
static void dump_text(const char* path);
static Instruction decode_instr(const void* loc);
static uint32_t swap_endian(uint32_t instr);
static void text_get_info(const char* path);

static long text_offset = 0;
static long text_size = 0;
static void* text_bindump = NULL;
static Instruction* instructions = NULL;

#define INSTR_LENGTH 4

void disassembler_init(const char* path) {
    generate_masks();
    text_get_info(path);
    printf("Text section offset: 0x%lx\n", text_offset);
    printf("Text section size: 0x%lx\n", text_size);

    dump_text(path);
    disassemble();
}

static void decode_operands(Instruction& i) {
    const char* enc = opcodes[i.opcode].encoding;
    int p = 0; // string offset
    int bit = 0; // opcode offset
    int len = 0; // current operand length
    OperandType type = OperandType::None; // last evaluated operand

    while(enc[p] != '\0') {
        if (enc[p] == ':') {
            // separator
            p++;
            if (type != OperandType::None) {
                len++;
                bit++;
            }
        } else {
            if (type != OperandType::None) {
                uint32_t data = extract_bits(i.raw, 31 - bit, len);
                i.operands.emplace_back(31 - bit, len, data, type);
                len = 0;
                type = OperandType::None;
            }

            if (enc[p] == '-') {
                // reserved bit
                p++;
                bit++;
            } else {
                if (strncmp(&enc[p], "imm", 3) == 0) {
                    // of the form imm##
                    char* end;
                    strtol(&enc[p + 3], &end, 10);
                    p += (intptr_t)end - (intptr_t)&enc[p];
                    type = OperandType::Imm;
                } else if (strncmp(&enc[p], "Rd", 2) == 0) {
                    type = OperandType::Rd;
                    p += 2;
                } else if (strncmp(&enc[p], "Rn", 2) == 0) {
                    type = OperandType::Rn;
                    p += 2;
                } else if (strncmp(&enc[p], "Rt2", 3) == 0) {
                    type = OperandType::Rt2;
                    p += 3;
                } else if (strncmp(&enc[p], "Rt", 2) == 0) {
                    type = OperandType::Rt;
                    p += 2;
                } else if (strncmp(&enc[p], "cond", 4) == 0) {
                    type = OperandType::cond;
                    p += 4;
                } else {
                    char buf[64];
                    strcpy(buf, &enc[p]);
                    printf("unhandled operand: %s\n", strtok(buf, ":"));
                    return;
                }
            }
        }
    }

    if (type != OperandType::None) {
        uint32_t data = extract_bits(i.raw, 31 - bit, len);
        i.operands.emplace_back(31 - bit, len, data, type);
    }
}

static Instruction decode_instr(const void* loc) {
    uint32_t raw = *(uint32_t*) loc;
    Instruction i = {.opcode = bad};

    for (int a = 0; a < 1115; ++a) {
        uint32_t h = opcodes[a].hex;
        if ((raw & op_masks[a]) == h) {
            i.opcode = (Opcode)a;
            i.raw = raw;
            decode_operands(i);
            print_instr(i);
            break;
        }
    }

    return i;
}

static void disassemble(void) {
    unsigned long dis_size = 0x100 * sizeof(Instruction);
    void* pos = text_bindump;
    intptr_t end = (intptr_t)text_bindump + text_size;
    unsigned long instr_count = 0;
    instructions = (Instruction*) malloc(dis_size);

    while ((intptr_t)pos < end) {
        Instruction i = decode_instr(pos);

        // discard if instruction was decoded past end of section
        if ((intptr_t)pos + INSTR_LENGTH - 1 >= end) {
            printf("instruction error at %lx\n", text_offset + (intptr_t)pos - (intptr_t)text_bindump);
            break;
        }

        instr_count++;

        // mem check
        if (instr_count * sizeof(Instruction) > dis_size) {
            dis_size *= 2;
            instructions = (Instruction*) realloc(instructions, dis_size);
        }

        instructions[instr_count - 1] = i;
        pos = (void*) ((intptr_t) pos + INSTR_LENGTH);
    }

    printf("Done disassemble. instr_count %ld\n", instr_count);
}

/*
 * Dump code into memory
 */
static void dump_text(const char* path) {
    FILE* f = fopen(path, "r");
    text_bindump = malloc(text_size);
    fseek(f, text_offset, SEEK_SET);
    fread(text_bindump, text_size, 1, f);
    fclose(f);
}

static uint32_t __attribute__((unused)) swap_endian(uint32_t instr) {
    uint32_t result = 0;
    result = ((instr >> 24) & 0xFF) |
             ((instr << 8) & 0xFF0000) |
             ((instr >> 8) & 0xFF00) |
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
