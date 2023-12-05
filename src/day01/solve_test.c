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

CTEST(day01, example1) {
    const char *buf = "1abc2\n\
pqr3stu8vwx\n\
a1b2c3d4e5f\n\
treb7uchet\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("142", solution.part1);
    ASSERT_STR("142", solution.part2);
}

CTEST(day01, example2) {
    const char *buf = "two1nine\n\
eightwothree\n\
abcone2threexyz\n\
xtwone3four\n\
4nineeightseven2\n\
zoneight234\n\
7pqrstsixteen\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("281", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day01, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("54081", solution.part1);
    ASSERT_STR("54649", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
