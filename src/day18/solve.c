/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

static inline int hex_to_int(char c) {
    if (c >= '0' && c <= '9') { return c - '0'; }
    return c - 'a' + 10;
}

typedef struct {
    i64 x, y;
    i64 area;
    i64 boundary;
} data_t;

static inline void update_data(data_t *data, char direction, i64 steps) {
    int x_new = data->x, y_new = data->y;
    switch (direction) {
    case 'U': y_new -= steps; break;
    case 'D': y_new += steps; break;
    case 'R': x_new += steps; break;
    case 'L': x_new -= steps; break;
    }
    // Shoelace formula (https://en.wikipedia.org/wiki/Shoelace_formula)
    data->area += (data->x * y_new - x_new * data->y);
    data->boundary += steps;
    data->x = x_new;
    data->y = y_new;
}

static inline i64 calc_area(data_t *data) {
    // Pick's theorem (https://en.wikipedia.org/wiki/Pick%27s_theorem)
    return (data->area + data->boundary) / 2 + 1;
}

void solve(char *buf, size_t buf_size, Solution *result) {
    data_t p1 = {.boundary = 1}, p2 = {.boundary = 1};
    for (size_t pos = 0; pos < buf_size;) {
        char p1_direction = buf[pos++];
        pos++;
        int p1_steps = aoc_parse_nonnegative(buf, &pos);

        aoc_parse_seek(buf, &pos, '#');
        pos++;

        i64 p2_steps = 0;
        for (int i = 0; i < 5; i++) { p2_steps = 16 * p2_steps + hex_to_int(buf[pos++]); }
        char p2_direction;
        switch (buf[pos++]) {
        case '0': p2_direction = 'R'; break;
        case '1': p2_direction = 'D'; break;
        case '2': p2_direction = 'L'; break;
        case '3': p2_direction = 'U'; break;
        }

        aoc_parse_seek(buf, &pos, '\n');
        pos++;

        update_data(&p1, p1_direction, p1_steps);
        update_data(&p2, p2_direction, p2_steps);
    }

    i64 part1 = calc_area(&p1);
    i64 part2 = calc_area(&p2);

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
