/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "aoc/macros.h"
#include "hand.h"
#include "part1.h"

#define LT -1
#define EQ 0
#define GT 1

// The higher the value the stronger the card.
// Strength range is [0, 12].
static inline int card_strength_part2(char c) {
    // J -> 0, 2 -> 1, ..., 9 -> 8, rest is the same as in card_strength
    switch (c) {
    case 'T': return 9;
    case 'J': return 0;
    case 'Q': return 10;
    case 'K': return 11;
    case 'A': return 12;
    default: assert(c >= '2' && c <= '9'); return c - '1';
    }
}

static inline int break_tie_part2(Hand x, Hand y) {
    for (int i = 0; i < CARDS_PER_HAND; i++) {
        char lhs = x.cards[i], rhs = y.cards[i];
        if (lhs != rhs) { return card_strength_part2(lhs) < card_strength_part2(rhs) ? LT : GT; }
    }
    return EQ;
}

hand_strength Hand_compute_strength_part2(Hand h) {
    // all possible values a Joker can take on
    static char joker_values[] = {'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};

    int joker_idx[CARDS_PER_HAND];
    int joker_count = 0;
    for (int i = 0; i < CARDS_PER_HAND; i++) {
        if (h.cards[i] == 'J') { joker_idx[joker_count++] = i; }
    }
    // for each joker, try possible values;
    // note that it's always better to use the same value for a joker, so we
    // don't have to bother trying different values for different jokers within
    // the same hand
    hand_strength max_strength = 0;
    for (size_t i = 0; i < ARRAY_LENGTH(joker_values); i++) {
        char value = joker_values[i];
        // substitute joker within the hand
        for (int j = 0; j < joker_count; j++) { h.cards[joker_idx[j]] = value; }
        hand_strength strength = Hand_compute_strength(h);
        if (strength > max_strength) { max_strength = strength; }
    }
    return max_strength;
}

// return GT if x > y
static inline int Hand_compare_part2(Hand x, Hand y) {
    hand_strength x_strength = Hand_compute_strength_part2(x);
    hand_strength y_strength = Hand_compute_strength_part2(y);

    if (x_strength < y_strength) { return LT; }
    if (x_strength > y_strength) { return GT; }
    assert(x_strength == y_strength);
    return break_tie_part2(x, y);
}

#define SORT_NAME Hand_part2
#define SORT_TYPE Hand
#define SORT_CMP(x, y) (Hand_compare_part2(x, y))
#include "sort.h"
