#include "ut.h"

uint32_t extract_bits(uint32_t n, unsigned long bit, unsigned long count) {
    return ((1 << count) - 1) & (n >> bit);
}

uint32_t __attribute__((unused)) swap_endian(uint32_t instr) {
    uint32_t result = 0;
    result = ((instr >> 24) & 0xFF) | ((instr << 8) & 0xFF0000) | ((instr >> 8) & 0xFF00) |
             ((instr << 24) & 0xFF000000);
    return result;
}
