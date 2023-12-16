/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stddef.h>

typedef struct Solution {
    char part1[64];
    char part2[64];
} Solution;

void solve(char *buf, size_t buf_size, Solution *result);
int solve_input(const char *fname, Solution *result);
