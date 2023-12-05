/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Concept:
 *
 * This puzzle (part 2) was unusually tricky considering it is only the 5th
 * puzzle. My initial strategy was to employ a brute-force approach, moving from
 * seed to location, similar to part 1. However, this method required O(10^9)
 * operations, making it computationally useless. The key insight was
 * to reverse the process: starting from location 1, mapping backwards from
 * location to seed values. The solution emerges when a specific interval
 * encompasses the computed seed value.
 */

#include "str.h"

#include "aoc/all.h"
#include "solve.h"

#define MAX_MAPS 32

typedef struct {
    i64 dst; /* destination range start */
    i64 src; /* source range start */
    i64 len; /* range length */
} Map;

typedef struct {
    str from;
    str to;
    Map maps[MAX_MAPS];
    u16 map_len;
} Recipe;

typedef struct {
    i64 from;
    i64 to;
} Interval;

static inline bool Interval_contains(Interval *interval, i64 value) {
    return interval->from <= value && value <= interval->to;
}

static inline i64 Map_lookup(const Map *m, i64 number) {
    if (number >= m->src && number < m->src + m->len) {
        return m->dst + (number - m->src);
    }
    return number;
}

static inline Map Map_invert(const Map *m) {
    Map result = {.src = m->dst, .dst = m->src, .len = m->len};
    return result;
}

static inline i64 Recipe_lookup(Recipe *r, i64 number) {
    for (u16 i = 0; i < r->map_len; i++) {
        Map *m = &r->maps[i];
        i64 value = Map_lookup(m, number);
        if (value != number) { return value; };
    }
    return number;
}

void solve(const char *buf, size_t buf_size, Solution *result) {
    u16 seeds_count = 0, recipe_count = 0;
    i64 seeds[32];
    Recipe recipes[32];
    Recipe inv_recipes[32];

    size_t pos = 0;
    aoc_parse_seek(buf, &pos, ':');
    pos++;

    i64 tmp;
    while ((tmp = aoc_parse_nonnegative(buf, &pos)) >= 0) {
        seeds[seeds_count++] = tmp;
    }

    while (pos < buf_size) {
        while (buf[pos] == '\n') pos++;

        size_t start = pos;
        while (buf[pos] != '-') pos++;
        size_t end = pos;
        str from = str_ref_chars(&buf[start], end - start);

        pos += 4; // skip "-to-"

        start = pos;
        while (buf[pos] != ' ') pos++;
        end = pos;
        str to = str_ref_chars(&buf[start], end - start);

        aoc_parse_seek(buf, &pos, '\n');
        pos++;

        recipes[recipe_count].from = from;
        recipes[recipe_count].to = to;
        inv_recipes[recipe_count].from = to;
        inv_recipes[recipe_count].to = from;
        while (buf[pos] >= '0' && buf[pos] <= '9') {
            u16 idx = recipes[recipe_count].map_len;
            Map *map = &recipes[recipe_count].maps[idx];
            map->dst = aoc_parse_nonnegative(buf, &pos);
            map->src = aoc_parse_nonnegative(buf, &pos);
            map->len = aoc_parse_nonnegative(buf, &pos);
            recipes[recipe_count].map_len++;

            idx = inv_recipes[recipe_count].map_len;
            inv_recipes[recipe_count].maps[idx] = Map_invert(map);
            inv_recipes[recipe_count].map_len++;

            pos++;
        }
        recipe_count++;
    }

    str start = str_lit("seed");
    str final = str_lit("location");
    i64 part1 = INT64_MAX;

    size_t optimal_path_index[32];
    size_t optimal_path_index_count = 0;

    log_debug("finding optimal path from seed to location");
    str from = start;
    while (!str_eq(from, final)) { // follow the path
        for (u16 j = 0; j < recipe_count; j++) {
            Recipe *r = &recipes[j];
            if (str_eq(from, r->from)) {
                optimal_path_index[optimal_path_index_count++] = j;
                from = r->to;
                break;
            }
        }
    }

    for (u16 i = 0; i < seeds_count; i++) {
        i64 value = seeds[i];
        for (size_t j = 0; j < optimal_path_index_count; j++) {
            Recipe *r = &recipes[optimal_path_index[j]];
            value = Recipe_lookup(r, value);
        }
        if (value < part1) {
            log_debug("new best seed: %ld", seeds[i]);
            part1 = value;
        }
    }

    // part 2
    Interval interval[32];
    size_t interval_count = 0;
    for (int i = 0; i < seeds_count; i += 2) {
        Interval inter = {.from = seeds[i], .to = seeds[i] + seeds[i + 1] - 1};
        interval[interval_count++] = inter;
    }

    // reverse search: location to seed
    i64 part2 = 0;
    for (i64 location = 1; part2 == 0; location++) {
        i64 seed = location;
        for (int j = optimal_path_index_count - 1; j >= 0; j--) {
            Recipe *r = &inv_recipes[optimal_path_index[j]];
            seed = Recipe_lookup(r, seed);
        }
        // check if this is contained in one the given seed intervals
        for (size_t i = 0; i < interval_count; i++) {
            if (Interval_contains(&interval[i], seed)) {
                part2 = location;
                break;
            }
        }
    }

    stbsp_snprintf(result->part1, sizeof(result->part1), "%ld", part1);
    stbsp_snprintf(result->part2, sizeof(result->part2), "%ld", part2);
}

int solve_input(const char *fname, Solution *result) {
    char buf[1 << 14];
    int n = aoc_io_read_input(fname, buf, sizeof(buf));
    if (n <= 0) {
        fprintf(stderr, "Failed to read %s\n", fname);
        return -1;
    }
    solve(buf, n, result);
    return 0;
}
