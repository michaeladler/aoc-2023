#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "aoc/types.h"

#define OPERATIONAL '.'
#define DAMAGED '#'
#define UNKNOWN '?'

#define MAX_SPRINGS 32
#define MAX_GROUPS 8
#define MAX_UNKNOWNS 32

typedef struct {
    u32 bitset; // 1 means damaged, 0 means ok
    int len;

} spring_t;

static inline void spring_set_damaged(spring_t *spring, size_t pos) { spring->bitset |= 1 << pos; }
static inline void spring_unset_damaged(spring_t *spring, size_t pos) { spring->bitset &= ~(1 << pos); }

static inline bool spring_is_damaged(spring_t *spring, size_t pos) { return (spring->bitset & (1 << pos)) != 0; }

static inline int spring_count_damaged(spring_t spring) { return __builtin_popcount(spring.bitset); }
