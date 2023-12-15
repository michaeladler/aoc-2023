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

CTEST(day14, example) {
    const char *buf = "O....#....\n\
O.OO#....#\n\
.....##...\n\
OO.#O....O\n\
.O.....O#.\n\
O.#..O.#.#\n\
..O..#O..O\n\
.......O..\n\
#....###..\n\
#OO..#....\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("136", solution.part1);
    // ASSERT_STR("64", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day14, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("106997", solution.part1);
    ASSERT_STR("99641", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
