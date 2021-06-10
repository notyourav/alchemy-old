#include "ut.h"

uint32_t extract_bits(uint32_t n, unsigned long bit, unsigned long count) {
    return ((1 << count) - 1) & (n >> bit);
}
