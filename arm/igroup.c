#include "igroup.h"

//const IGroup group_reserved = 0;
//const IGroup group_unallocated = 1;
//const IGroup group_sve = 2;
//const IGroup group_dataProcessingImmediate = 3;
//const IGroup group_branchesAndExceptions = 4;
//const IGroup group_loadsAndStores = 5;
//const IGroup group_dataProcessingRegister = 6;
//const IGroup group_dataProcessingScalar = 7;

/*
 * Lookup table for Arm64 instruction groups
 * bits 24-29
 */
const IGroup iGroups[] = {
        [0] = group_reserved,
        [1] = group_unallocated,
        [2] = group_sve,
        [3] = group_unallocated,
        [4] = group_loadsAndStores,
        [5] = group_dataProcessingRegister,
        [6] = group_loadsAndStores,
        [7] = group_dataProcessingScalar,
        [8] = group_dataProcessingImmediate,
        [9] = group_dataProcessingImmediate,
        [10] = group_branchesAndExceptions,
        [11] = group_branchesAndExceptions,
        [12] = group_loadsAndStores,
        [13] = group_dataProcessingRegister,
        [14] = group_loadsAndStores,
        [15] = group_dataProcessingScalar,
};

const char* iGroups_debug[] = {
        [0] = "reserved",
        [1] = "unallocated",
        [2] = "sve",
        [3] = "data processing immediate",
        [4] = "branches and exceptions",
        [5] = "loads and stores",
        [6] = "data processing register",
        [7] = "data processing scalar",
};
