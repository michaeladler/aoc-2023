/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "aoc/all.h"
#include "solve.h"

static const char ONE[] = {'o', 'n', 'e'};
static const char TWO[] = {'t', 'w', 'o'};
static const char THREE[] = {'t', 'h', 'r', 'e', 'e'};
static const char FOUR[] = {'f', 'o', 'u', 'r'};
static const char FIVE[] = {'f', 'i', 'v', 'e'};
static const char SIX[] = {'s', 'i', 'x'};
static const char SEVEN[] = {'s', 'e', 'v', 'e', 'n'};
static const char EIGHT[] = {'e', 'i', 'g', 'h', 't'};
static const char NINE[] = {'n', 'i', 'n', 'e'};

void solve(const char *buf, size_t buf_size, Solution *result) {
    int part1 = 0, part2 = 0;
    size_t pos = 0;
    int digits[64] = {0}, digits2[64] = {0};
    int alpha = 0, beta = 0;
    while (pos < buf_size) {
        int value = buf[pos];
        if (value >= '0' && value <= '9') {
            digits[alpha++] = value - '0';
            digits2[beta++] = value - '0';
        } else if (value >= 'a' && value <= 'z') {
            //  attempt to parse word
            if (pos + sizeof(ONE) < buf_size &&
                memcmp(&buf[pos], ONE, sizeof(ONE)) == 0) {
                digits2[beta++] = 1;
            } else if (pos + sizeof(TWO) < buf_size &&
                       memcmp(&buf[pos], TWO, sizeof(TWO)) == 0) {
                digits2[beta++] = 2;
            } else if (pos + sizeof(THREE) < buf_size &&
                       memcmp(&buf[pos], THREE, sizeof(THREE)) == 0) {
                digits2[beta++] = 3;
            } else if (pos + sizeof(FOUR) < buf_size &&
                       memcmp(&buf[pos], FOUR, sizeof(FOUR)) == 0) {
                digits2[beta++] = 4;
            } else if (pos + sizeof(FIVE) < buf_size &&
                       memcmp(&buf[pos], FIVE, sizeof(FIVE)) == 0) {
                digits2[beta++] = 5;
            } else if (pos + sizeof(SIX) < buf_size &&
                       memcmp(&buf[pos], SIX, sizeof(SIX)) == 0) {
                digits2[beta++] = 6;
            } else if (pos + sizeof(SEVEN) < buf_size &&
                       memcmp(&buf[pos], SEVEN, sizeof(SEVEN)) == 0) {
                digits2[beta++] = 7;
            } else if (pos + sizeof(EIGHT) < buf_size &&
                       memcmp(&buf[pos], EIGHT, sizeof(EIGHT)) == 0) {
                digits2[beta++] = 8;
            } else if (pos + sizeof(NINE) < buf_size &&
                       memcmp(&buf[pos], NINE, sizeof(NINE)) == 0) {
                digits2[beta++] = 9;
            }
        } else if (value == '\n') {
            part1 += digits[0] * 10 + digits[MAX(0, alpha - 1)];
            part2 += digits2[0] * 10 + digits2[MAX(0, beta - 1)];
            // reset state
            alpha = 0, beta = 0;
        }
        pos++;
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
