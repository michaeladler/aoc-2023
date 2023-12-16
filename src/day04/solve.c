/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

// forward decl (to speed up incremental compilation)
void int_tim_sort(int *dst, const size_t size);

void solve(char *buf, size_t buf_size, Solution *result) {
    int part1 = 0, part2 = 0, card_id = 1, tmp;
    int copies[256] = {0};
    size_t pos = 0;

    while (pos < buf_size) {
        copies[card_id]++;
        int wc[16], mc[32], wc_count = 0, mc_count = 0;

        aoc_parse_seek(buf, &pos, ':');
        pos++;

        while ((tmp = aoc_parse_nonnegative(buf, &pos)) >= 0) { wc[wc_count++] = tmp; }

        aoc_parse_seek(buf, &pos, '|');
        pos++;

        while ((tmp = aoc_parse_nonnegative(buf, &pos)) >= 0) { mc[mc_count++] = tmp; }

        int_tim_sort(wc, wc_count);
        int_tim_sort(mc, mc_count);
        int match_count = 0;
        for (int i = 0, j = 0; i < wc_count && j < mc_count;) {
            if (wc[i] == mc[j]) {
                match_count++, i++, j++;
            } else if (wc[i] < mc[j]) {
                i++;
            } else {
                j++;
            }
        }
        if (match_count > 0) {
            part1 += 1 << (match_count - 1);
            int instances = copies[card_id];
            for (int c = card_id + 1; c <= card_id + match_count; c++) { copies[c] += instances; }
        }

        pos++; // newline
        card_id++;
    }
    for (size_t i = 0; i < ARRAY_LENGTH(copies); i++) { part2 += copies[i]; }

    aoc_itoa(part1, result->part1, 10);
    aoc_itoa(part2, result->part2, 10);
}

int solve_input(const char *fname, Solution *result) {
    char buf[1 << 15];
    int n = aoc_io_read_input(fname, buf, sizeof(buf));
    if (n <= 0) {
        fprintf(stderr, "Failed to read %s\n", fname);
        return -1;
    }
    solve(buf, n, result);
    return 0;
}
