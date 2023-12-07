/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#include "part1.h"
#include "part2.h"

void solve(const char *buf, size_t buf_size, Solution *result) {
    i64 part1 = 0, part2 = 0;
    size_t pos = 0;

    Hand hand[1000];
    size_t hand_count = 0;

    // parser
    while (pos < buf_size) {
        for (int i = 0; i < CARDS_PER_HAND; i++) {
            hand[hand_count].cards[i] = buf[pos++];
        }
        hand[hand_count].bid = aoc_parse_nonnegative(buf, &pos);
        hand_count++;
        pos++; // newline
    }

    // part 1
    Hand_tim_sort(hand, hand_count); // weakest hand is first
    for (size_t i = 0; i < hand_count; i++) {
        i64 rank = i + 1, bid = hand[i].bid;
        part1 += rank * bid;
    }

    // part 2
    Hand_part2_tim_sort(hand, hand_count);
    for (size_t i = 0; i < hand_count; i++) {
        i64 rank = i + 1, bid = hand[i].bid;
        part2 += rank * bid;
    }

    aoc_itoa(part1, result->part1, 10);
    aoc_itoa(part2, result->part2, 10);
}

int solve_input(const char *fname, Solution *result) {
    char buf[1 << 14];
    int n = aoc_io_read_input(fname, buf, sizeof(buf));
    if (n <= 0) {
        fprintf(stderr, "Failed to read %s\n", fname);
        return -1;
    }
    solve(buf, n, result);
    return 0;
}
