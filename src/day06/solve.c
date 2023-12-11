/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"
#include <math.h>

#define N 32

static void extract_digits(i32 number, i64 out[], i32 *out_count) {
    i32 idx = *out_count, tmp[32], tmp_idx = 0;
    while (number != 0) {
        tmp[tmp_idx++] = number % 10;
        number = number / 10;
    }
    for (int i = tmp_idx - 1; i >= 0; i--) { out[idx++] = tmp[i]; }
    *out_count = idx;
}

static inline i64 count_values(i64 time_avail, i64 record) {
    // Find all integers v such that v * (time_avail - v) > record
    double discriminant = time_avail * time_avail - 4 * record;
    i64 l = (i64)floor((time_avail - sqrt(discriminant)) / 2);
    i64 u = (i64)ceil((time_avail + sqrt(discriminant)) / 2);

    // check if lower is ok
    if (l * (time_avail - l) <= record) { // not ok
        l++;
    }

    // check if upper is ok
    if (u * (time_avail - u) <= record) { // not ok
        u--;
    }
    return u - l + 1;
}

void solve(const char *buf, size_t _unused_ buf_size, Solution *result) {
    i32 time[N], record_dist[N];
    int n = 0;

    { // parser
        size_t pos = 0;
        aoc_parse_seek(buf, &pos, ':');
        pos++;

        i64 tmp;
        while ((tmp = aoc_parse_nonnegative(buf, &pos)) >= 0) { time[n++] = tmp; }
        aoc_parse_seek(buf, &pos, ':');
        pos++;
        for (int i = 0; i < n; i++) { record_dist[i] = aoc_parse_nonnegative(buf, &pos); }
    }

    i64 part1 = 1, part2 = 1;
    // part 1
    for (int i = 0; i < n; i++) {
        i32 time_avail = time[i], record = record_dist[i];
        i64 count = count_values(time_avail, record);
        if (count > 0) part1 *= count;
    }

    // part 2
    i64 time_digits[16], record_digits[16];
    int time_digits_count = 0, record_digits_count = 0;
    for (int i = 0; i < n; i++) { extract_digits(time[i], time_digits, &time_digits_count); }

    for (int i = 0; i < n; i++) { extract_digits(record_dist[i], record_digits, &record_digits_count); }

    i64 time_avail = 0, record = 0;
    for (int i = 0; i < time_digits_count; i++) { time_avail = (time_avail * 10) + time_digits[i]; }
    for (int i = 0; i < record_digits_count; i++) { record = (record * 10) + record_digits[i]; }

    i64 count = count_values(time_avail, record);
    if (count > 0) part2 *= count;

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
