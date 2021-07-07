#pragma once

#include <cstdio>

class CompilerInstance {
    const char* mName;
    const char* mPath;
    const char* mArgs;

    FILE* invoke(FILE* src);
};