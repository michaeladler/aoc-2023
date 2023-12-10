/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define CTEST_MAIN

#include "ctest.h"
#include "solve.h"

CTEST(day10, example1) {
    const char *buf = ".....\n\
.S-7.\n\
.|.|.\n\
.L-J.\n\
.....\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("4", solution.part1);
    // ASSERT_STR("0", solution.part2);
}

CTEST(day10, example2) {
    const char *buf = "..F7.\n\
.FJ|.\n\
SJ.L7\n\
|F--J\n\
LJ...\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("8", solution.part1);
    // ASSERT_STR("0", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day10, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("6951", solution.part1);
    // ASSERT_STR("0", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
