/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>

#include "solve.h"

#ifndef DAY
#error "Please define DAY"
#endif

int main(int argc, char *argv[]) {
    const char *fname = argc > 1 ? argv[1] : "input/" DAY ".txt";
    Solution solution;
    if (solve_input(fname, &solution)) {
        fprintf(stderr, DAY ": no solution found!\n");
        return 1;
    }
    printf("Part1: %s\nPart2: %s\n", solution.part1, solution.part2);
    return 0;
}
