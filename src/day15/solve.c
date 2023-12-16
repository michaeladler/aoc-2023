/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#include <slice99.h>

static inline u32 hash(CharSlice99 s) {
    u32 value = 0;
    for (size_t i = 0; i < s.len; i++) {
        char c = *CharSlice99_get(s, i);
        value += c;
        value *= 17;
        value %= 256;
    }
    log_debug("hash of %.*s is %u", s.len, s.ptr, value);
    return value;
}

void solve(char *buf, size_t buf_size, Solution *result) {
    u64 part1 = 0, part2 = 0;
    CharSlice99 input = CharSlice99_from_ptrdiff(buf, &buf[buf_size - 2]); // no '\0', no '\n'

    size_t start = 0;
    size_t pos = 0;
    while (pos < input.len) {
        if (*CharSlice99_get(input, pos) == ',') {
            CharSlice99 s = CharSlice99_sub(input, start, pos);
            part1 += hash(s);
            start = pos + 1;
        }
        pos++;
    }
    CharSlice99 s = CharSlice99_sub(input, start, pos + 1);
    part1 += hash(s);

    snprintf(result->part1, sizeof(result->part1), "%lu", part1);
    snprintf(result->part2, sizeof(result->part2), "%lu", part2);
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
