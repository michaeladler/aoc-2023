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

CTEST(day09, example) {
    const char *buf = "0 3 6 9 12 15\n\
1 3 6 10 15 21\n\
10 13 16 21 30 45\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("114", solution.part1);
    ASSERT_STR("2", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day09, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("1882395907", solution.part1);
    ASSERT_STR("1005", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
