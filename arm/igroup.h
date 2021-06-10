#ifndef ALCHEMY_IGROUP_H
#define ALCHEMY_IGROUP_H

#include <stdint.h>

typedef enum {
    group_reserved = 0,
    group_unallocated = 1,
    group_sve = 2,
    group_dataProcessingImmediate = 3,
    group_branchesAndExceptions = 4,
    group_loadsAndStores = 5,
    group_dataProcessingRegister = 6,
    group_dataProcessingScalar = 7,
} IGroup;

//typedef uint8_t IGroup;

//const IGroup group_reserved;
//const IGroup group_unallocated;
//const IGroup group_sve;
//const IGroup group_dataProcessingImmediate;
//const IGroup group_branchesAndExceptions;
//const IGroup group_loadsAndStores;
//const IGroup group_dataProcessingRegister;
//const IGroup group_dataProcessingScalar;

const IGroup iGroups[16];
const char* iGroups_debug[8];

#endif //ALCHEMY_IGROUP_H
