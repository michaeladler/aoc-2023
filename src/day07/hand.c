#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "aoc/macros.h"
#include "hand.h"

#define LT -1
#define EQ 0
#define GT 1

// The higher the value the stronger the card.
// Strength range is [0, 12].
static inline int card_strength(char c) {
    switch (c) {
    case 'T': return 8;
    case 'J': return 9;
    case 'Q': return 10;
    case 'K': return 11;
    case 'A': return 12;
    default: assert(c >= '2' && c <= '9'); return c - '2';
    }
}

static inline int break_tie(Hand x, Hand y) {
    // Start by comparing the first card in each hand. If these cards are
    // different, the hand with the stronger first card is considered stronger.
    // If the first card in each hand have the same label, however, then move
    // on to considering the second card in each hand. If they differ, the hand
    // with the higher second card wins; otherwise, continue with the third
    // card in each hand, then the fourth, then the fifth.
    for (int i = 0; i < CARDS_PER_HAND; i++) {
        char lhs = x.cards[i], rhs = y.cards[i];
        if (lhs != rhs) {
            return card_strength(lhs) < card_strength(rhs) ? LT : GT;
        }
    }
    return EQ;
}

hand_strength Hand_compute_strength(Hand h) {
    int occurences[13] = {0};
    for (int i = 0; i < CARDS_PER_HAND; i++) {
        occurences[card_strength(h.cards[i])]++;
    }

    bool has_quintet = false, has_quadruple = false, has_triple = false;
    int pair_count = 0;

    for (size_t i = 0; i < ARRAY_LENGTH(occurences); i++) {
        if (occurences[i] == 5) {
            has_quintet = true;
        } else if (occurences[i] == 4) {
            has_quadruple = true;
        } else if (occurences[i] == 3) {
            has_triple = true;
        } else if (occurences[i] == 2) {
            pair_count++;
        }
    }

    if (has_quintet) return FIVE_KIND;
    if (has_quadruple) return FOUR_KIND;
    if (has_triple && pair_count > 0) return FULL_HOUSE;
    if (has_triple) return THREE_KIND;
    if (pair_count == 2) return TWO_PAIR;
    if (pair_count == 1) return ONE_PAIR;
    return HIGH_CARD;
}

// return GT if x > y
static inline int Hand_compare(Hand x, Hand y) {

    hand_strength x_strength = Hand_compute_strength(x);
    hand_strength y_strength = Hand_compute_strength(y);

    if (x_strength < y_strength) { return LT; }
    if (x_strength > y_strength) { return GT; }
    assert(x_strength == y_strength);
    return break_tie(x, y);
}

#define SORT_NAME Hand
#define SORT_TYPE Hand
#define SORT_CMP(x, y) (Hand_compare(x, y))
#include "sort.h"
