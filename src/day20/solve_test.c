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

CTEST(day20, example1) {
    char *buf = "broadcaster -> a, b, c\n\
%a -> b\n\
%b -> c\n\
%c -> inv\n\
&inv -> a\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("32000000", solution.part1);
    // ASSERT_STR("0", solution.part2);
}

CTEST(day20, example2) {
    char *buf = "broadcaster -> a\n\
%a -> inv, con\n\
&inv -> b\n\
%b -> con\n\
&con -> output\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("11687500", solution.part1);
    // ASSERT_STR("0", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day20, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("929810733", solution.part1);
    // ASSERT_STR("0", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
