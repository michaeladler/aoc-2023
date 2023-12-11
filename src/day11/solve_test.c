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

CTEST(day11, example) {
    const char *buf = "...#......\n\
.......#..\n\
#.........\n\
..........\n\
......#...\n\
.#........\n\
.........#\n\
..........\n\
.......#..\n\
#...#.....\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("374", solution.part1);
    // ASSERT_STR("0", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST_SKIP(day11, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("9274989", solution.part1);
    // ASSERT_STR("0", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
