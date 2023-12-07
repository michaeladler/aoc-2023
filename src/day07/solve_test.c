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

CTEST(day07, example) {
    const char *buf = "32T3K 765\n\
T55J5 684\n\
KK677 28\n\
KTJJT 220\n\
QQQJA 483\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("6440", solution.part1);
    ASSERT_STR("5905", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day07, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("253910319", solution.part1);
    ASSERT_STR("254083736", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
