/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#define MAX_DIM 100
#define ROLLING 'O'
#define FIXED_POINT '#'
#define EMPTY '.'

static char platform[MAX_DIM][MAX_DIM];

static void tilt_north(int rows, int cols) {
    for (int y = 1; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            char c = platform[y][x];
            if (c != ROLLING) continue;
            int new_y = y;
            while (new_y >= 1 && platform[new_y - 1][x] == EMPTY) { new_y--; }
            if (new_y != y) {
                log_debug("row %d, col %d moves to row %d", y, x, new_y);
                platform[new_y][x] = c;
                platform[y][x] = EMPTY;
            }
        }
    }
}

static i64 compute_total_load(int rows, int cols) {
    i64 result = 0;
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            char c = platform[y][x];
            if (c == ROLLING) { result += cols - y; }
        }
    }
    return result;
}

static void print_platform(int rows, int cols) {
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) { printf("%c", platform[y][x]); }
        puts("");
    }
}

void solve(const char *buf, size_t buf_size, Solution *result) {
    size_t pos = 0;
    int rows, cols;
    { // parser
        int y = 0, x = 0;
        while (pos < buf_size) {
            char c = buf[pos];
            if (c == '\n') {
                cols = x;
                x = 0, y++, pos++;
                continue;
            }
            platform[y][x++] = c;
            pos++;
        }
        rows = y;
    }

    tilt_north(rows, cols);

    i64 part1, part2 = 0;
    part1 = compute_total_load(rows, cols);

    snprintf(result->part1, sizeof(result->part1), "%ld", part1);
    snprintf(result->part2, sizeof(result->part2), "%ld", part2);
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
