#include "disassembler/program.h"
#include "disassembler/disassembler.h"
#include "elf.h"
#include <fstream>

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

Program::Program(const char* path, Disassembler& dis) {
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
                mTextInfo.offset = shdrs[i].sh_offset;
                mTextInfo.size = shdrs[i].sh_size;
                break;
            }
        }
    } else {
        // MACH-O
        char buffer[64];
        char cmd[64];
        FILE* pipe = nullptr;

        sprintf(cmd, "objdump -h %s", path);
        pipe = popen(cmd, "r");
        while (fgets(buffer, sizeof buffer, pipe) != nullptr) {
            if (strstr(buffer, "__text") != nullptr) {
                char* end = nullptr;
                char size[64];
                char vma[64];

                sscanf(buffer, "%*s %*s %s %s", size, vma);
                mTextInfo.offset = strtol(vma, &end, 16) - 0x100000000;
                mTextInfo.size = strtol(size, &end, 16);
            }
        }
    }
    assert(mTextInfo.offset != 0);
    assert(mTextInfo.size != 0);
    assert(mTextInfo.size % 4 == 0);

    fs.seekg(mTextInfo.offset, std::ios::beg);
    char* buf2 = (char*)malloc(mTextInfo.size);
    fs.read(buf2, mTextInfo.size);

    mInstructions = dis.disassemble({(uint32_t*)buf2, mTextInfo.size / 4});
    createFunctions();
}

void Program::createFunctions() {
}
