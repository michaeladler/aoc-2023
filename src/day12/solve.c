/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#include "backtrack.h"

void solve(const char *buf, size_t buf_size, Solution *result) {
    int part1 = 0, part2 = 0;

    for (size_t pos = 0; pos < buf_size;) { // parser
        struct BT bt = {.solution_count = 0};
        {
            size_t i;
            for (i = 0; buf[pos] != ' '; i++) {
                if (buf[pos] == DAMAGED) {
                    spring_set_damaged(&bt.spring, i);
                } else if (buf[pos] == UNKNOWN) {
                    bt.idx_unknown[bt.idx_unknown_count++] = i;
                }
                pos++;
            }
            log_debug("spring length: %d", i);
            bt.spring.len = i;
        }
        pos++; // whitespace
        while (buf[pos] != '\n') {
            i64 value = aoc_parse_nonnegative(buf, &pos);
            assert(value >= 0);
            bt.group[bt.group_count++] = value;
            if (buf[pos] == ',') pos++;
        }

        // part 1
        int a[MAX_UNKNOWNS];
        backtrack(a, -1, &bt);

        part1 += bt.solution_count;

        pos++;
    }

    aoc_itoa(part1, result->part1, 10);
    aoc_itoa(part2, result->part2, 10);
}

int solve_input(const char *fname, Solution *result) {
    char buf[1 << 15];
    int n = aoc_io_read_input(fname, buf, sizeof(buf));
    if (n <= 0) {
        fprintf(stderr, "Failed to read %s\n", fname);
        return -1;
    }
    solve(buf, n, result);
    return 0;
}
