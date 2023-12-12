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

CTEST(day12, example) {
    const char *buf = "???.### 1,1,3\n\
.??..??...?##. 1,1,3\n\
?#?#?#?#?#?#?#? 1,3,1,6\n\
????.#...#... 4,1,1\n\
????.######..#####. 1,6,5\n\
?###???????? 3,2,1\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("21", solution.part1);
    // ASSERT_STR("0", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day12, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("7163", solution.part1);
    // ASSERT_STR("0", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
