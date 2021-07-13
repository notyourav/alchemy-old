#pragma once

#include "aim/features.h"
#include "arm/instruction.h"

struct InlineCandidate {
    Instruction* start;
    Instruction* end;
};
