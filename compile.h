#pragma once

#include "disassembler/program.h"

class Compiler {
public:
    Compiler(const char* name, const char* path, const char* args) {
        mName = name;
        mPath = path;
        mArgs = args;
    }

    Program* invoke(const char* path);

private:
    const char* mName;
    const char* mPath;
    const char* mArgs;
};
