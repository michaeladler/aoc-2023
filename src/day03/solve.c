/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#define MAX_NUMBERS 150
#define MAX_SYMBOLS 4096
#define EMPTY -1

typedef struct {
    int x;
    int y;
    char value;
} Symbol;

static inline bool is_symbol(char s) {
    return !(s >= '0' && s <= '9') && s != '.';
}

static inline int collect(int8_t numbers[MAX_NUMBERS][MAX_NUMBERS], int cols,
                          int candidate_x, int candidate_y) {
    // go left as far as possible to find start digit
    while (candidate_x - 1 >= 0 &&
           numbers[candidate_y][candidate_x - 1] != EMPTY) {
        candidate_x--;
    }
    int value = numbers[candidate_y][candidate_x];
    // reset to prevent collecting twice
    numbers[candidate_y][candidate_x] = EMPTY;
    for (int i = candidate_x + 1; i < cols; i++) {
        if (numbers[candidate_y][i] == EMPTY) break;
        value *= 10;
        value += numbers[candidate_y][i];
        numbers[candidate_y][i] = EMPTY;
    }
    return value;
}

void solve(const char *buf, size_t buf_size, Solution *result) {
    int cols = 0;
    while (buf[cols] != '\n') cols++;
    int rows = 0;

    int8_t numbers[MAX_NUMBERS][MAX_NUMBERS];
    memset(numbers, EMPTY, sizeof(numbers));
    Symbol symbols[MAX_SYMBOLS];
    size_t symbols_idx = 0;

    size_t pos = 0;
    while (pos < buf_size) {
        for (int x = 0; x < cols; x++) {
            if (is_symbol(buf[pos])) {
                Symbol s = {.x = x, .y = rows, .value = buf[pos]};
                symbols[symbols_idx++] = s;
            } else if (buf[pos] >= '0' && buf[pos] <= '9') {
                numbers[rows][x] = buf[pos] - '0';
            }
            pos++;
        }
        if (pos < buf_size && buf[pos] == '\n') {
            rows++;
            pos++;
        }
    }

    int part1 = 0, part2 = 0;
    for (size_t i = 0; i < symbols_idx; i++) {
        int y = symbols[i].y, x = symbols[i].x;
        bool is_star = symbols[i].value == '*';

        int values[8];
        int value_idx = 0;

        Point2D deltas[] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0},
                            {1, 0},   {-1, 1}, {0, 1},  {1, 1}};

        for (size_t i = 0; i < ARRAY_LENGTH(deltas); i++) {
            int candidate_x = x + deltas[i].x, candidate_y = y + deltas[i].y;
            if (candidate_x >= 0 && candidate_y >= 0 && candidate_x < cols &&
                candidate_y < rows &&
                numbers[candidate_y][candidate_x] != EMPTY) {
                values[value_idx++] =
                    collect(numbers, cols, candidate_x, candidate_y);
            }
        }
        for (int i = 0; i < value_idx; i++) { part1 += values[i]; }
        if (is_star && value_idx == 2) { part2 += values[0] * values[1]; }
    }

    aoc_itoa(part1, result->part1, 10);
    aoc_itoa(part2, result->part2, 10);
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
