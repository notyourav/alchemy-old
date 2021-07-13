#include "arm/instruction.h"

enum class PInstType {
    NOP,
    ADD,
    MUL,
    SHIFT,
    LOAD,
    STORE,
    MOVE,
    CMP,
    BCOND,
    BRANCH,
    CALL,
    RET,
};
extern std::map<PInstType, const char*> inst_type_names;

struct PInst {
    std::vector<PInstType> types;
    Instruction instruction;
};
