#include "disassembler/pcode.h"

std::map<PInstType, const char*> inst_type_names = {
    {PInstType::NOP, "NOP"},       {PInstType::ADD, "ADD"},   {PInstType::MUL, "MUL"},
    {PInstType::SHIFT, "SHIFT"},   {PInstType::LOAD, "LOAD"}, {PInstType::STORE, "STORE"},
    {PInstType::MOVE, "MOVE"},     {PInstType::CMP, "CMP"},   {PInstType::BCOND, "BCOND"},
    {PInstType::BRANCH, "BRANCH"}, {PInstType::CALL, "CALL"}, {PInstType::RET, "RET"},
};
