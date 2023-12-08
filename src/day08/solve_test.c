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

CTEST(day08, example1) {
    const char *buf = "RL\n\
\n\
AAA = (BBB, CCC)\n\
BBB = (DDD, EEE)\n\
CCC = (ZZZ, GGG)\n\
DDD = (DDD, DDD)\n\
EEE = (EEE, EEE)\n\
GGG = (GGG, GGG)\n\
ZZZ = (ZZZ, ZZZ)\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("2", solution.part1);
    // ASSERT_STR("0", solution.part2);
}

CTEST(day08, example2) {
    const char *buf = "LLR\n\
\n\
AAA = (BBB, BBB)\n\
BBB = (AAA, ZZZ)\n\
ZZZ = (ZZZ, ZZZ)\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("6", solution.part1);
    // ASSERT_STR("0", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day08, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("16897", solution.part1);
    // ASSERT_STR("0", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
