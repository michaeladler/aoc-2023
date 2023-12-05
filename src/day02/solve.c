/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "aoc/all.h"
#include "solve.h"

void solve(const char *buf, size_t buf_size, Solution *result) {
    int part1 = 0, part2 = 0;
    size_t pos = 0;
    int game_id = 1;
    while (pos < buf_size) {
        log_debug("Game %d", game_id);
        bool game_ok = true;
        long red_min = 0, green_min = 0, blue_min = 0;
        while (pos < buf_size && buf[pos] != ':') pos++;
        while (true) {
            aoc_parse_skip_ws(buf, &pos);
            int amount = aoc_parse_nonnegative(buf, &pos);
            aoc_parse_skip_ws(buf, &pos);
            if (buf[pos] == 'r') {
                log_debug("red %d", amount);
                red_min = MAX(red_min, amount);
                if (amount > 12) game_ok = false;
            } else if (buf[pos] == 'g') {
                log_debug("green %d", amount);
                green_min = MAX(green_min, amount);
                if (amount > 13) game_ok = false;
            } else if (buf[pos] == 'b') {
                log_debug("blue %d", amount);
                blue_min = MAX(blue_min, amount);
                if (amount > 14) game_ok = false;
            }
            // skip word
            while (buf[pos] >= 'a' && buf[pos] <= 'z') pos++;
            if (buf[pos] == '\n') {
                if (game_ok) part1 += game_id;
                log_debug("min rgb: %d %d %d", red_min, green_min, blue_min);
                part2 += red_min * green_min * blue_min;
                pos++;
                game_id++;
                break;
            } else if (buf[pos] == ';') {
                log_debug("next set");
            }
            pos++;
        }
        pos++;
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
