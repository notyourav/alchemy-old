#include "disassembler.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include "disassembler/program.h"
#include "elf.h"
#include "ut.h"

static void text_get_info(const char* path);
// static long text_offset = 0;
// static size_t text_size = 0;

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

static Instruction decode_instr(uint32_t instr, intptr_t address) {
    Instruction i = {.opcode = bad};

    for (int a = 0; a < 1115; ++a) {
        uint32_t h = opcodes[a].hex;
        if ((instr & op_masks[a]) == h) {
            i.opcode = (Opcode)a;
            i.raw = instr;
            i.address = address;
            decode_operands(i);
            print_instr(i);
            break;
        }
    }

    return i;
}

// TODO: support variable length instructions.
const Program& Disassembler::init(std::span<uint32_t> data) {
    int addr = mProgram.text_offset;
    for (auto& i : data) {
        Instruction instr = decode_instr(i, addr);
        addr += sizeof(data.front());

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
    printf("Text section offset: 0x%lx\n", mProgram.text_offset);
    printf("Text section size: 0x%lx\n", mProgram.text_size);

    FILE* f = fopen(path, "r");
    uint32_t* dump = (uint32_t*)malloc(mProgram.text_size);
    fseek(f, mProgram.text_offset, SEEK_SET);
    fread(dump, mProgram.text_size, 1, f);
    fclose(f);

    init({dump, text_size / sizeof(uint32_t)});
    free(dump);
    return mProgram;
}

// get ELF file header
static Elf64_Ehdr* get_ehdr(std::fstream& f) {
    Elf64_Ehdr* hdr = (Elf64_Ehdr*)malloc(sizeof(Elf64_Ehdr));
    f.seekg(0, std::ios::beg);
    f.read((char*)hdr, sizeof(Elf64_Ehdr));
    return hdr;
}

// get ELF section headers
static Elf64_Shdr* get_shdrs(std::fstream& f, Elf64_Ehdr* hdr) {
    Elf64_Shdr* shdr = (Elf64_Shdr*)malloc(hdr->e_shentsize * hdr->e_shnum);
    f.seekg(hdr->e_shoff, std::ios::beg);
    f.read((char*)shdr, hdr->e_shentsize * hdr->e_shnum);
    return shdr;
}

// get ELF section header string table (section names)
static const char* get_shstrtab(std::fstream& f, Elf64_Ehdr* hdr, Elf64_Shdr* shdrs) {
    Elf64_Shdr* sh_symtab = &shdrs[hdr->e_shstrndx];
    char* strtab = (char*)malloc(sh_symtab->sh_size);
    f.seekg(sh_symtab->sh_offset, std::ios::beg);
    f.read(strtab, sh_symtab->sh_size);
    return strtab;
}

static void text_get_info(const char* path) {
    std::fstream fs(path, std::fstream::in | std::fstream::binary);
    char buf[4];
    fs.read(buf, 4);
    if (buf[0] == 0x7f && buf[1] == 'E' && buf[2] == 'L' && buf[3] == 'F') {
        // ELF
        Elf64_Ehdr* hdr = get_ehdr(fs);
        Elf64_Shdr* shdrs = get_shdrs(fs, hdr);
        const char* shstrtab = get_shstrtab(fs, hdr, shdrs);

        for (int i = 0; i < hdr->e_shnum; ++i) {
            Elf64_Shdr* h = &shdrs[i];
            const char* name = &shstrtab[h->sh_name];
            if (strcmp(name, ".text") == 0) {
                text_offset = shdrs[i].sh_offset;
                text_size = shdrs[i].sh_size;
                break;
            }
        }
    } else {
        // MACH-O
        char buffer[64];
        char cmd[64];
        FILE* pipe = NULL;

#define DUMP_CMD "objdump -h %s"
#define DUMP_SENTINEL "__text"

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
    }
    assert(text_offset != 0);
    assert(text_size != 0);
}
