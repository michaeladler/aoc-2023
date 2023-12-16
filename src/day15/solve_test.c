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

CTEST(day15, example) {
    char *buf = "rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("1320", solution.part1);
    ASSERT_STR("145", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day15, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("515495", solution.part1);
    ASSERT_STR("229349", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
