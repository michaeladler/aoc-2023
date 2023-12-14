/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#define MAX_ROWS 32
#define MAX_COLS 32

#define ASH '.'
#define ROCK '#'

typedef struct {
    char data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} grid_t;

/*
static void print_grid(const grid_t *grid) {
    for (int y = 0; y < grid->rows; y++) {
        for (int x = 0; x < grid->cols; x++) { printf("%c", grid->data[y][x]); }
        puts("");
    }
}
*/

static bool is_perfect_vertical(const grid_t *grid, int x) {
    // x denotes the position of the > in ><
    int delta = 0;
    while (1) {
        int x_left = x - delta;
        int x_right = x + 1 + delta;
        if (x_left < 0 || x_right >= grid->cols) break;
        // compare columns
        for (int y = 0; y < grid->rows; y++) {
            if (grid->data[y][x_left] != grid->data[y][x_right]) return false;
        }
        delta++;
    }
    return true;
}

static bool is_perfect_horizontal(const grid_t *grid, int y) {
    // y denotes the position of the v
    int delta = 0;
    while (1) {
        int y_top = y - delta;
        int y_down = y + 1 + delta;
        if (y_top < 0 || y_down >= grid->rows) break;
        // compare rows
        for (int x = 0; x < grid->cols; x++) {
            if (grid->data[y_top][x] != grid->data[y_down][x]) return false;
        }
        delta++;
    }
    return true;
}

void solve(const char *buf, size_t buf_size, Solution *result) {
    i64 part1 = 0, part2 = 0;
    size_t pos = 0;

    while (pos < buf_size) {
        grid_t grid = {0};
        while (1) {
            int x = 0;
            while (buf[pos] != '\n') { grid.data[grid.rows][x++] = buf[pos++]; }
            grid.rows++;
            grid.cols = x;
            pos++; // newline
            if (pos >= buf_size || buf[pos] == '\n') {
                // current pattern is over
                pos++;
                break;
            }
        }
        log_debug(">> grid has %d rows, %d cols", grid.rows, grid.cols);

        for (int x = 0; x < grid.cols - 1; x++) {
            if (is_perfect_vertical(&grid, x)) {
                int col = x + 1;
                log_debug("found perfect vertical reflection between %d and %d", col, col + 1);
                part1 += col;
                break;
            }
        }
        for (int y = 0; y < grid.rows - 1; y++) {
            if (is_perfect_horizontal(&grid, y)) {
                int row = y + 1;
                log_debug("found perfect horizontal reflection between %d and %d", row, row + 1);
                part1 += 100 * row;
                break;
            }
        }
    }

    snprintf(result->part1, sizeof(result->part1), "%ld", part1);
    snprintf(result->part2, sizeof(result->part2), "%ld", part2);
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
