#include <stdbool.h>
#include <string.h>
#include "iclass.h"
#include "ut.h"

typedef struct {
    const char* match;
    uint32_t offset;
} OpMask;

#define MASK_END {"", 0},
#define MAXOPS 6

//OpMask op_masks[][MAXOPS] = {
//        [class_simdLoadStoreMultiple] = {
//                {"0x00", 28},
//                {"1", 26},
//                {"00", 23},
//                {"00000", 16},
//                MASK_END
//        },
//        [class_simdLoadStoreMultiplePostIndex] = {
//                {"0x00", 28},
//                {"1", 26},
//                {"01", 23},
//                {"0xxxxx", 16},
//                MASK_END
//        },
//        [class_simdLoadStoreSingle] = {
//                {"0x00", 28},
//                {"1", 26},
//                {"10", 23},
//                {"x00000", 16},
//                MASK_END
//        },
//        [class_simdLoadStoreSinglePostIndex] = {
//                {"0x00", 28},
//                {"1", 26},
//                {"11", 23},
//                MASK_END
//        },
//        [class_loadStoreMemoryTags] = {
//                {"1101", 28},
//                {"0", 26},
//                {"1x", 23},
//                {"1xxxxx", 16},
//                MASK_END
//        },
//        [class_loadStoreExclusive] = {
//                {"xx00", 28},
//                {"0", 26},
//                {"0x", 23},
//                MASK_END
//        },
//        [class_LDAPR] = {
//                {"xx01", 28},
//                {"0", 26},
//                {"1x", 23},
//                {"0xxxxx", 16},
//                {"00", 10},
//                MASK_END
//        },
//        [class_loadRegisterLiteral] = {
//                {"xx01", 28},
//                {"0x",   23},
//                MASK_END
//        },
//        [class_loadStorePairNoAllocate] = {
//                {"xx10", 28},
//                {"00", 23},
//                MASK_END
//        },
//        [class_loadStorePairPostIndex] = {
//                {"xx10", 28},
//                {"01", 23},
//                MASK_END
//        },
//        [class_loadStorePairOffset] = {
//                {"xx10", 28},
//                {"10", 23},
//                MASK_END
//        },
//        [class_loadStorePairPreIndex] = {
//                {"xx10", 28},
//                {"11", 23},
//                MASK_END
//        },
//        [class_loadStoreRegister] = {
//                {"xx11", 28},
//                {"0x", 23},
//                {"0xxxxx", 16},
//                {"00", 10},
//                MASK_END
//        },
//        [class_loadStoreRegisterPostIndex] = {
//                {"xx11", 28},
//                {"0x", 23},
//                {"0xxxxx", 16},
//                {"01", 10},
//                MASK_END
//        },
//        [class_loadStoreRegisterUnprivileged] = {
//                {"xx11", 28},
//                {"0x", 23},
//                {"0xxxxx", 16},
//                {"10", 10},
//                MASK_END
//        },
//        [class_loadStoreRegisterPreIndex] = {
//                {"xx11", 28},
//                {"0x", 23},
//                {"0xxxxx", 16},
//                {"11", 10},
//                MASK_END
//        },
//        [class_atomicOperations] = {
//                {"xx11", 28},
//                {"0x", 23},
//                {"1xxxxx", 16},
//                {"00", 10},
//                MASK_END
//        },
//        [class_loadStoreRegisterOffset] = {
//                {"xx11", 28},
//                {"0x", 23},
//                {"1xxxxx", 16},
//                {"10", 10},
//                MASK_END
//        },
//        [class_loadStoreRegisterPac] = {
//                {"xx11", 28},
//                {"0x", 23},
//                {"1xxxxx", 16},
//                {"x1", 10},
//                MASK_END
//        },
//        [class_loadStoreRegisterUnsigned] = {
//                {"xx11", 28},
//                {"1x", 23},
//                MASK_END
//        },
//};

/*
 *  mask an instruction wildcard-style
 */
bool mask_opcode(uint32_t instruction, OpMask masks[]) {
    for (int i = 0; i < MAXOPS; ++i) {
        if (masks[i].match[0] == '\0')
            return false;
        uint32_t bitcnt = strlen(masks[i].match);
        uint32_t bits = extract_bits(instruction, masks[i].offset, bitcnt);
        for (uint32_t b = 0; b < bitcnt; ++b) {
            switch (masks[i].match[b]) {
                case '0':
                    if (extract_bits(bits, b, 1) != 0)
                        return false;
                    break;
                case '1':
                    if (extract_bits(bits, b, 1) != 1)
                        return false;
                    break;
                case 'x':
                    break;
            }
        }
    }
    return true;
}

//IClass get_class(uint32_t instruction, IGroup group) {
//    switch (group) {
//        case group_loadsAndStores:
//            for (int i = __loadsAndStores + 1; i < __dataProcessing; ++i) {
//                if (mask_opcode(instruction, op_masks[i]))
//                    return i;
//            }
//            break;
//        case group_reserved:
//            break;
//        case group_unallocated:
//            break;
//        case group_sve:
//            break;
//        case group_dataProcessingImmediate:
//            break;
//        case group_branchesAndExceptions:
//            break;
//        case group_dataProcessingRegister:
//            break;
//        case group_dataProcessingScalar:
//            break;
//    }
//    return class_unallocated;
//}

const char* iClass_debug[] = {
        /* reserved */
        "class_undefined",
        /* unallocated */
        "class_unallocated",
        /* data processing immediate */
        "class_pcRelativeAddressing",
        "class_addSubtractImmediate",
        "class_addSubtractImmediateTags",
        "class_logicalImmediate",
        "class_moveWideImmediate",
        "class_bitfield",
        "class_extract",
        /* branches and exceptions */
        "class_conditionalBranchImmediate",
        "class_exceptionGeneration",
        "class_systemInstructionsRegister",
        "class_hints",
        "class_barriers",
        "class_pstate",
        "class_systemInstructions",
        "class_SystemRegisterMove",
        "class_unconditionalBranchRegister",
        "class_unconditionalBranchImmediate",
        "class_compareAndBranchImmediate",
        "class_testAndBranchImmediate",
        /* loads and stores */
        "class_simdLoadStoreMultiple",
        "class_simdLoadStoreMultiplePostIndex",
        "class_simdLoadStoreSingle",
        "class_simdLoadStoreSinglePostIndex",
        "class_loadStoreMemoryTags",
        "class_loadStoreExclusive",
        "class_LDAPR",
        "class_loadRegisterLiteral",
        "class_loadStorePairNoAllocate",
        "class_loadStorePairPostIndex",
        "class_loadStorePair",
        "class_loadStorePairPreIndex",
        "class_loadStoreRegister",
        "class_loadStoreRegisterPostIndex",
        "class_loadStoreRegisterUnprivileged",
        "class_loadStoreRegisterPreIndex",
        "class_atomicOperations",
        "class_loadStoreRegisterOffset",
        "class_loadStoreRegisterPac",
        "class_loadStoreRegisterUnsigned",
        /* data processing */
        "class_dataProcessing2Source",
        "class_dataProcessing1Source",
        "class_logicalShifted",
        "class_addSubShifted",
        "class_addSubExtended",
        "class_addSubCarry",
        "class_rotateRightFlags",
        "class_evalFlags",
        "class_conditionalCompareRegister",
        "class_conditionalCompareImmediate",
        "class_conditionalSelect",
        "class_dataProcessing3Source",
        /* float and SIMD */
//        class_cryptoAES,
//        class_crypto3SHA,
//        class_crypto2SHA,
//        class_simdScalarCopy,
//        class_simdScalarThreeSameFP16,
//        class_simdScalarTwoRegisterMiscFP16,
//        class_simdScalarThreeSameExtra,
//        class_simdScalarTwoRegisterMisc,
//        class_simdScalarPairwise,
//        class_simdScalarThreeDifferent,
//        class_simdScalarThreeSame,
//        class_simdScalarShiftImmediate,
//        class_simdScalarIndexedElement,
//        class_simdTableLookup,
//        class_simdPermute,
//        class_simdExtract,
//        class_simdCopy,
//        class_simdThreeSameFP16,
//        class_simdTwoRegisterMiscFP16,
//        class_simdThreeRegisterExtension,
//        class_simdTwoRegisterMisc,
//        class_simdAcrossLanes,
//        class_simdThreeDifferent,
//        class_simdThreeSame,
//        class_simdModifiedImmediate,
//        class_simdShiftImmediate,
//        class_simdVectorIndexedElement,
//        class_cryptoThreeRegisterImm2,
//        class_cryptoThreeRegisterSHA512,
//        class_cryptoFourRegister,
//        class_cryptoTwoRegisterSHA512,
//        class_convertFloatFixed,
//        class_convertFloatInteger,
//        class_floatDataProcessing1Source,
//        class_floatCompare,
//        class_floatImmediate,
//        class_floatConditionalCompare,
//        class_floatDataProcessing2Source,
//        class_floatConditionalSelect,
//        class_floatDataProcessing3Source,
};
