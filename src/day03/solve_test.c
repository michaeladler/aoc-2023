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

CTEST(day03, example) {
    const char *buf = "467..114..\n\
...*......\n\
..35..633.\n\
......#...\n\
617*......\n\
.....+.58.\n\
..592.....\n\
......755.\n\
...$.*....\n\
.664.598..\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("4361", solution.part1);
    ASSERT_STR("467835", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day03, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("520135", solution.part1);
    ASSERT_STR("72514855", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
