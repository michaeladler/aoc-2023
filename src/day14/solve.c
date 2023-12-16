/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#include <inttypes.h>
#include <xxhash.h>

#define MAX_DIM 100
#define ROLLING 'O'
#define FIXED_POINT '#'
#define EMPTY '.'

#define MAX_CYCLE_LEN 512

#define P
#define T XXH128_hash_t
#include <ust.h>

static char platform[MAX_DIM][MAX_DIM];

static void tilt_north(int rows, int cols) {
    for (int y = 1; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            char c = platform[y][x];
            if (c != ROLLING) continue;
            int new_y = y;
            while (new_y - 1 >= 0 && platform[new_y - 1][x] == EMPTY) { new_y--; }
            if (new_y != y) {
                // log_debug("row %d, col %d moves to row %d", y, x, new_y);
                platform[new_y][x] = c;
                platform[y][x] = EMPTY;
            }
        }
    }
}

static void tilt_south(int rows, int cols) {
    for (int y = rows - 2; y >= 0; y--) {
        for (int x = 0; x < cols; x++) {
            char c = platform[y][x];
            if (c != ROLLING) continue;
            int new_y = y;
            while (new_y + 1 < rows && platform[new_y + 1][x] == EMPTY) { new_y++; }
            if (new_y != y) {
                // log_debug("row %d, col %d moves to row %d", y, x, new_y);
                platform[new_y][x] = c;
                platform[y][x] = EMPTY;
            }
        }
    }
}

static void tilt_east(int rows, int cols) {
    for (int x = cols - 2; x >= 0; x--) {
        for (int y = 0; y < rows; y++) {
            char c = platform[y][x];
            if (c != ROLLING) continue;
            int new_x = x;
            while (new_x + 1 < cols && platform[y][new_x + 1] == EMPTY) { new_x++; }
            if (new_x != x) {
                // log_debug("row %d, col %d moves to col %d", y, x, new_x);
                platform[y][new_x] = c;
                platform[y][x] = EMPTY;
            }
        }
    }
}

static void tilt_west(int rows, int cols) {
    for (int x = 1; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            char c = platform[y][x];
            if (c != ROLLING) continue;
            int new_x = x;
            while (new_x - 1 >= 0 && platform[y][new_x - 1] == EMPTY) { new_x--; }
            if (new_x != x) {
                // log_debug("row %d, col %d moves to col %d", y, x, new_x);
                platform[y][new_x] = c;
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

static size_t hash(XXH128_hash_t *x) {
    const size_t prime = 31;
    size_t hash = 17;
    hash = hash * prime + (size_t)x->low64;
    hash = hash * prime + (size_t)x->high64;
    return hash;
}

static int equal(XXH128_hash_t *a, XXH128_hash_t *b) { return a->low64 == b->low64 && a->high64 == b->high64; }

void solve(char *buf, size_t buf_size, Solution *result) {
    size_t pos = 0;
    int rows, cols = 0;
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

    i64 part1, part2 = 0;

    XXH128_hash_t hashes[MAX_CYCLE_LEN];
    int hashes_count = 0;
    _cleanup_(ust_XXH128_hash_t_free) ust_XXH128_hash_t seen = ust_XXH128_hash_t_init(hash, equal);
    ust_XXH128_hash_t_reserve(&seen, MAX_CYCLE_LEN);

    XXH128_hash_t duplicate;

    // find cycle
    int cycle = 0;
    for (; cycle < MAX_CYCLE_LEN; cycle++) {
        tilt_north(rows, cols);
        if (cycle == 0) { part1 = compute_total_load(rows, cols); }

        tilt_west(rows, cols);
        tilt_south(rows, cols);
        tilt_east(rows, cols);

        XXH128_hash_t value = XXH3_128bits(&platform[0][0], rows * cols);
        hashes[hashes_count++] = value;

        ust_XXH128_hash_t_node *node = ust_XXH128_hash_t_find(&seen, value);
        if (node != NULL) {
            duplicate = value;
            break;
        } else {
            ust_XXH128_hash_t_insert(&seen, value);
        }
    }

    int cycle_start = 0;
    while (!equal(&hashes[cycle_start], &duplicate)) { cycle_start++; }
    int cycle_len = 1;
    while (!equal(&hashes[cycle_start + cycle_len], &duplicate)) { cycle_len++; }
    log_debug("cycle starts at %d and has length %d", cycle_start, cycle_len);
    // we are at the begining of the cycle
    log_debug("we are at cycle %d", cycle);

    int last_cycle = 1000000000 - 1;

    int delta = last_cycle - cycle;
    log_debug("%d cycles left to compute", delta);

    int rem = last_cycle % delta; // shortcut
    for (int i = 0; i <= rem; i++) {
        tilt_north(rows, cols);
        tilt_west(rows, cols);
        tilt_south(rows, cols);
        tilt_east(rows, cols);
    }
    part2 = compute_total_load(rows, cols);

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
