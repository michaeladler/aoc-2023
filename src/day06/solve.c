/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#define N 4

void solve(const char *buf, size_t _unused_ buf_size, Solution *result) {
    i64 part1 = 1, part2 = 0;

    i32 time[N], record_dist[N];
    int n = 0;

    { // parser
        size_t pos = 0;
        aoc_parse_seek(buf, &pos, ':');
        pos++;

        i64 tmp;
        while ((tmp = aoc_parse_nonnegative(buf, &pos)) >= 0) {
            time[n++] = tmp;
        }

        aoc_parse_seek(buf, &pos, ':');
        pos++;
        for (int i = 0; i < n; i++) {
            record_dist[i] = aoc_parse_nonnegative(buf, &pos);
        }
    }

    for (int i = 0; i < n; i++) {
        i32 time_avail = time[i], record = record_dist[i];
        i64 count = 0;
        for (i32 wait = 1; wait < time_avail; wait++) {
            i32 v = wait, t = time_avail - wait;
            i32 d = v * t;
            if (d > record) {
                log_debug("i=%d: waiting for %d ms beats the record", i, wait);
                count++;
            }
        }
        if (count > 0) { part1 *= count; }
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
