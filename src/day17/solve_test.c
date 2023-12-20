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

CTEST(day17, example) {
    char *buf = "2413432311323\n\
3215453535623\n\
3255245654254\n\
3446585845452\n\
4546657867536\n\
1438598798454\n\
4457876987766\n\
3637877979653\n\
4654967986887\n\
4564679986453\n\
1224686865563\n\
2546548887735\n\
4322674655533\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("102", solution.part1);
    // ASSERT_STR("0", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day17, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("861", solution.part1);
    // ASSERT_STR("0", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
