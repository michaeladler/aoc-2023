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

CTEST(day05, example) {
    const char *buf = "seeds: 79 14 55 13\n\
\n\
seed-to-soil map:\n\
50 98 2\n\
52 50 48\n\
\n\
soil-to-fertilizer map:\n\
0 15 37\n\
37 52 2\n\
39 0 15\n\
\n\
fertilizer-to-water map:\n\
49 53 8\n\
0 11 42\n\
42 0 7\n\
57 7 4\n\
\n\
water-to-light map:\n\
88 18 7\n\
18 25 70\n\
\n\
light-to-temperature map:\n\
45 77 23\n\
81 45 19\n\
68 64 13\n\
\n\
temperature-to-humidity map:\n\
0 69 1\n\
1 0 69\n\
\n\
humidity-to-location map:\n\
60 56 37\n\
56 93 4\n";
    Solution solution;
    solve(buf, strlen(buf), &solution);
    ASSERT_STR("35", solution.part1);
    ASSERT_STR("46", solution.part2);
}

#ifdef HAVE_INPUTS
CTEST(day05, real) {
    Solution solution;
    solve_input("input/" DAY ".txt", &solution);
    ASSERT_STR("1181555926", solution.part1);
    ASSERT_STR("37806486", solution.part2);
}
#endif

int main(int argc, const char *argv[]) { return ctest_main(argc, argv); }
