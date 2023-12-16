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

CTEST(day16, example) {
    char *buf = ".|...\\....\n"
                "|.-.\\.....\n"
                ".....|-...\n"
                "........|.\n"
                ".........\n"
                ".........\\\n"
                "..../.\\\\..\n"
                ".-.-/..|..\n"
                ".|....-|.\\\n"
                "..//.|....\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("46", solution.part1);
    ASSERT_STR("51", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day16, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("7632", solution.part1);
    ASSERT_STR("8023", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
