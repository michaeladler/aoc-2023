/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

void solve(const char *buf, size_t buf_size, Solution *result) {
    int part1 = 0, part2 = 0;
    size_t pos = 0;
    while (pos < buf_size) {
        // have fun
        (void)buf[pos++];
    }
    stbsp_snprintf(result->part1, sizeof(result->part1), "%d", part1);
    stbsp_snprintf(result->part2, sizeof(result->part2), "%d", part2);
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
