/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#define MAX_POINTS 1024

static const i64 expand_p2 = 1000000 - 1;

void solve(char *buf, size_t buf_size, Solution *result) {

    i64 part1 = 0, part2 = 0;
    size_t pos = 0;

    Point2D point[MAX_POINTS];
    i32 point_count = 0, empty_row_count = 0, empty_col_count = 0, rows = 0, cols = 0;
    i32 empty_row[MAX_POINTS], empty_col[MAX_POINTS];

    { // parser
        i32 x = 0, y = 0;
        bool is_empty_row = true;
        while (pos < buf_size) {
            char c = buf[pos++];
            if (c == '\n') {
                if (is_empty_row) { empty_row[empty_row_count++] = y; }
                is_empty_row = true;
                cols = x;
                x = 0;
                y++;
                continue;
            }
            if (c == '#') {
                is_empty_row = false;
                Point2D p = {.x = x, .y = y};
                point[point_count++] = p;
            }
            x++;
        }
        rows = y;
    }

    for (i32 x = 0; x < cols; x++) {
        bool is_empty = true;
        for (i32 y = 0; y < rows; y++) {
            char c = INDEX(buf, x, y, cols + 1);
            if (c != '.') {
                is_empty = false;
                break;
            }
        }
        if (is_empty) { empty_col[empty_col_count++] = x; }
    }

    for (int i = 0; i < point_count; i++) {
        Point2D a = point[i];
        for (int j = i + 1; j < point_count; j++) {
            Point2D b = point[j];
            i64 p1_dist = Point2D_manhattan(a, b);
            i64 p2_dist = p1_dist;

            { // account for empty cols
                i64 p1_count = 0, p2_count = 0;
                i32 lower_x = MIN(a.x, b.x), upper_x = MAX(a.x, b.x);
                for (i32 k = 0; k < empty_col_count; k++) {
                    i32 c = empty_col[k];
                    if (c > lower_x && c < upper_x) {
                        p1_count++;
                        p2_count += expand_p2;
                    }
                }
                p1_dist += p1_count;
                p2_dist += p2_count;
            }
            { // account for empty rows
                i64 p1_count = 0, p2_count = 0;
                i32 lower_y = MIN(a.y, b.y);
                i32 upper_y = MAX(a.y, b.y);
                for (i32 k = 0; k < empty_row_count; k++) {
                    i32 r = empty_row[k];
                    if (r > lower_y && r < upper_y) {
                        p1_count++;
                        p2_count += expand_p2;
                    }
                }
                p1_dist += p1_count;
                p2_dist += p2_count;
            }
            part1 += p1_dist;
            part2 += p2_dist;
        }
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
