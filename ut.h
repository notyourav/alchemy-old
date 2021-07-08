#pragma once

#include <stdint.h>

#ifdef __cplusplus
#define EXT extern "C"
#else
#define EXT
#endif

EXT uint32_t extract_bits(uint32_t n, unsigned long bit, unsigned long count);
EXT uint32_t swap_endian(uint32_t instr);
