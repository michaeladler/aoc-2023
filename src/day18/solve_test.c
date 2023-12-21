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

CTEST(day18, example) {
    char *buf = "R 6 (#70c710)\n\
D 5 (#0dc571)\n\
L 2 (#5713f0)\n\
D 2 (#d2c081)\n\
R 2 (#59c680)\n\
D 2 (#411b91)\n\
L 5 (#8ceee2)\n\
U 2 (#caa173)\n\
L 1 (#1b58a2)\n\
U 2 (#caa171)\n\
R 2 (#7807d2)\n\
U 3 (#a77fa3)\n\
L 2 (#015232)\n\
U 2 (#7a21e3)\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("62", solution.part1);
    ASSERT_STR("952408144115", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day18, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("52231", solution.part1);
    ASSERT_STR("57196493937398", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
