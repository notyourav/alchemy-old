#include "compile.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include "disassembler/disassembler.h"

Program* Compiler::invoke(const char* path) {
    auto temp_path = std::filesystem::temp_directory_path();

    char obj_path[L_tmpnam];
    sprintf(obj_path, "%s", temp_path.append<const char*>("tmp.XXXXXXXX").c_str());

    if (mkstemp(obj_path) == -1) {
        assert(false);
    }

    char buf[L_tmpnam * 4];
    sprintf(buf, "%s %s %s -o %s", mPath, mArgs, path, obj_path);
    auto* p = popen(buf, "r");

    assert(p);

    char line[1024];
    while (fgets(line, 1024, p) != nullptr) {
        printf("(compile) %s", line);
    }
    pclose(p);

    auto dis = Disassembler();
    return new Program(obj_path, dis);
}
