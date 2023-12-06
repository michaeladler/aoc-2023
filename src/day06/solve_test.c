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

CTEST(day06, example) {
    const char *buf = "Time:      7  15   30\n\
Distance:  9  40  200\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("288", solution.part1);
    ASSERT_STR("0", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST_SKIP(day06, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("0", solution.part1);
    ASSERT_STR("0", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
