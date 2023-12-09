/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#define MAX_HISTORY 32

void solve(const char *buf, size_t buf_size, Solution *result) {
    i64 part1 = 0, part2 = 0;
    size_t pos = 0;

    const size_t base_idx = 1;
    while (pos < buf_size) {
        i64 differences[MAX_HISTORY][MAX_HISTORY] = {0};
        size_t history_count = 0;

        bool ok;
        i64 tmp;
        while ((ok = aoc_parse_integer(buf, &pos, &tmp)) == true) {
            differences[0][base_idx + history_count] = tmp;
            history_count++;
        }
        pos++; // newline

        {
            // compute difference sequences
            size_t depth = 1;
            while (depth < history_count) {
                bool all_zero = true;
                // there are history_count - depth items at depth
                for (size_t i = 0; i < history_count - depth; i++) {
                    i64 value = differences[depth - 1][base_idx + i + 1] -
                                differences[depth - 1][base_idx + i];
                    if (value != 0) all_zero = false;
                    differences[depth][base_idx + i] = value;
                }
                if (all_zero) { break; }
                depth++;
            }
            log_debug("all zero at depth %ld", depth);

            // compute next value in sequence (bubble up)
            do {
                depth--;
                size_t idx = history_count - depth; // position of item to add
                i64 value = differences[depth][base_idx + idx - 1] +
                            differences[depth + 1][base_idx + idx - 1];
                log_debug("depth %ld: adding item %ld in pos %ld", depth, value,
                          idx);
                differences[depth][base_idx + idx] = value;
            } while (depth != 0);

            part1 += differences[0][base_idx + history_count];
        }
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
