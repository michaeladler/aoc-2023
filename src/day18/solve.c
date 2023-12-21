/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

void solve(char *buf, size_t buf_size, Solution *result) {
    int part1 = 0, part2 = 0;

    int y = 0, x = 0;
    int area = 0, boundary = 1;
    for (size_t pos = 0; pos < buf_size;) {
        char d = buf[pos++];
        pos++;
        int steps = aoc_parse_nonnegative(buf, &pos);

        aoc_parse_seek(buf, &pos, '#');
        pos++;
        char color[6];
        for (int i = 0; i < 6; i++) { color[i] = buf[pos++]; }

        aoc_parse_seek(buf, &pos, '\n');
        pos++;

        // part 1
        int x_new = x, y_new = y;
        switch (d) {
        case 'U': y_new -= steps; break;
        case 'D': y_new += steps; break;
        case 'R': x_new += steps; break;
        case 'L': x_new -= steps; break;
        }

        // Shoelace formula (https://en.wikipedia.org/wiki/Shoelace_formula)
        area += (x * y_new - x_new * y);
        boundary += steps;

        x = x_new, y = y_new;
    }

    // Pick's theorem (https://en.wikipedia.org/wiki/Pick%27s_theorem)
    part1 = (area + boundary) / 2 + 1;

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
