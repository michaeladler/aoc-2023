/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#define N 32

static void extract_digits(i32 number, i64 out[], i32 *out_count) {
    i32 idx = *out_count;

    i32 tmp[32];
    int tmp_idx = 0;
    while (number != 0) {
        i32 dig = number % 10;
        number = (number - dig) / 10;
        tmp[tmp_idx++] = dig;
    }
    for (int i = tmp_idx - 1; i >= 0; i--) { out[idx++] = tmp[i]; }
    *out_count = idx;
}

void solve(const char *buf, size_t _unused_ buf_size, Solution *result) {
    i64 part1 = 1, part2 = 1;

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

    // part 1
    for (int i = 0; i < n; i++) {
        i32 time_avail = time[i], record = record_dist[i];
        i64 count = 0;
        for (i32 wait = 1; wait < time_avail; wait++) {
            i32 v = wait, t = time_avail - wait;
            if (v * t > record) { count++; }
        }
        if (count > 0) { part1 *= count; }
    }

    // part 2
    i64 time_digits[16], record_digits[16];
    int time_digits_count = 0, record_digits_count = 0;
    for (int i = 0; i < n; i++) {
        extract_digits(time[i], time_digits, &time_digits_count);
    }

    for (int i = 0; i < n; i++) {
        extract_digits(record_dist[i], record_digits, &record_digits_count);
    }

    i64 time_avail = 0, record = 0;
    for (int i = 0; i < time_digits_count; i++) {
        time_avail = (time_avail * 10) + time_digits[i];
    }
    for (int i = 0; i < record_digits_count; i++) {
        record = (record * 10) + record_digits[i];
    }

    i64 count = 0;
    for (i64 wait = 1; wait < time_avail; wait++) {
        i64 v = wait, t = time_avail - wait;
        if (v * t > record) { count++; }
    }
    if (count > 0) { part2 *= count; }

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
