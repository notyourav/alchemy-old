#include "compile.h"

void test_compile() {
    Compiler compiler("clang", "clang", "-c -O2 -arch arm64");
    Program* p = compiler.invoke("../samples/elf/for_loops.c");

    assert(p != nullptr);
}
