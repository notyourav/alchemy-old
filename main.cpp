#include <cstdlib>
#include <cstdio>
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
    char*   path = NULL;
    int     opt;

    while ((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {
            case 'i':
                path = optarg;
                break;
        }
    }

    // input path was supplied
    if (path != NULL) {
        disassembler_init(path);

//        FILE*   file    = NULL;
//        char*   content = NULL;
//        long    size;
//
//        file = fopen(path, "r");
//        if (file == NULL) {
//            fprintf(stderr, "Input file could not be opened.\n");
//            return EXIT_FAILURE;
//        }
//
//        size = filesize(file);
//        content = malloc(size);
//        fread(content, size, 1, file);
//        printf("filesize %ld kb\n", size / 1024);
//        fclose(file);
//
//        init_disasm(content);
//        free(content);
    }

    return 0;
}
