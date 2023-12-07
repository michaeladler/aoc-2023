#pragma once

#include "aoc/types.h"
#include <stddef.h>

#define CARDS_PER_HAND 5

typedef struct {
    char cards[CARDS_PER_HAND];
    i32 bid;
} Hand;

typedef enum {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_KIND,
    FULL_HOUSE,
    FOUR_KIND,
    FIVE_KIND
} hand_strength;

hand_strength Hand_compute_strength(Hand h);

// forward decl
void Hand_tim_sort(Hand *dst, const size_t size);
