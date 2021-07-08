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

int main(int argc, char* argv[]) {
    char* path = NULL;
    int opt;

    while ((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {
        case 'i':
            path = optarg;
            break;
        }
    }

    // input path was supplied
    if (path != NULL) {
        Program p(path, new Disassembler());

        int addr = p.text_info.offset;
        for (auto& i : p.instructions) {
            printf("0x%x: ", addr);
            print_instr(i);
            addr += 4;
        }
    }

    return 0;
}
