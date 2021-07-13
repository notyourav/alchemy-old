#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "disassembler/disassembler.h"

long filesize(FILE* f) {
    long pos;
    long result;

    pos = ftell(f);
    fseek(f, 0L, SEEK_END);
    result = ftell(f);
    fseek(f, pos, SEEK_SET);
    return result;
}

void print_dis(const char* path) {
    // input path was supplied
    if (path != NULL) {
        Program p(path, new Disassembler());

        int addr = p.text_offset();
        for (const auto& i : p.instructions()) {
            printf("0x%x:\n\t", addr);
            print_instr(i.instruction);
            for (PInstType j : i.types)
                printf("\t%s", inst_type_names[j]);
            printf("\n");
            addr += 4;
        }
    }
}

int main(int argc, char* argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "d:")) != -1) {
        switch (opt) {
        case 'd':
            print_dis(optarg);
            break;
        }
    }

    return 0;
}
