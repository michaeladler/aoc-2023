/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"
#include "aoc/types.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include <xxhash.h>

#define OPERATIONAL '.'
#define DAMAGED '#'
#define UNKNOWN '?'

#define MAX_SPRINGS 128
#define MAX_GROUPS 32
#define MAX_UNKNOWNS 64

typedef struct {
    char *spring;
    int spring_size;

    int size[MAX_GROUPS]; /* immutable so we can just copy the pointer */
    int size_count;
    int group_size;

    size_t value;
} CacheItem;

size_t CacheItem_hash(CacheItem *item) {
    XXH64_hash_t spring_hash = XXH3_64bits(item->spring, item->spring_size);

    size_t result = 17;
    result = 31 * result + item->spring_size;
    result = 31 * result + spring_hash;
    result = 31 * result + item->size_count;
    for (int i = 0; i < item->size_count; i++) { result = 31 * result + item->size[i]; }
    result = 31 * result + item->group_size;
    return result;
}

int CacheItem_equal(CacheItem *lhs, CacheItem *rhs) {
    return lhs->size_count == rhs->size_count && lhs->group_size == rhs->group_size &&
           lhs->spring_size == rhs->spring_size && memcmp(lhs->size, rhs->size, lhs->size_count) == 0 &&
           memcmp(lhs->spring, rhs->spring, lhs->spring_size) == 0;
}

#define P
#define T CacheItem
#include <ust.h>

static size_t find_solutions(ust_CacheItem *cache, char *spring, int spring_size, int *size, int size_count,
                             int group_size) {
    // check cache for answer
    CacheItem item = {.spring = spring, .spring_size = spring_size, .size_count = size_count, .group_size = group_size};
    memcpy(&item.size, size, size_count);

    ust_CacheItem_node *node = ust_CacheItem_find(cache, item);
    if (node != NULL) { return node->key.value; }

    if (spring[0] == '\0') {
        if (size_count == 1 && size[0] == group_size) { return 1; }
        if (size_count == 0 && group_size == 0) { return 1; }
        return 0;
    }

    size_t count = 0;
    char symbols[3];
    if (spring[0] == '?') {
        symbols[0] = OPERATIONAL;
        symbols[1] = DAMAGED;
        symbols[2] = '\0'; // marker
    } else {
        symbols[0] = spring[0];
        symbols[1] = '\0';
    }
    for (int i = 0; symbols[i] != '\0'; i++) {
        if (symbols[i] == DAMAGED) { // expand group
            count += find_solutions(cache, &spring[1], spring_size - 1, size, size_count, group_size + 1);
            continue;
        }
        assert(symbols[i] == OPERATIONAL);
        if (group_size > 0) {                              // current group has ended now
            if (size_count > 0 && size[0] == group_size) { // only continue if group size matches the expected one
                count += find_solutions(cache, &spring[1], spring_size - 1, &size[1], size_count - 1, 0);
            }
        } else {
            count += find_solutions(cache, &spring[1], spring_size - 1, size, size_count, 0);
        }
    }

    // save item in cache
    item.value = count;
    ust_CacheItem_insert(cache, item);

    return count;
}

void solve(const char *buf, size_t buf_size, Solution *result) {
    size_t part1 = 0, part2 = 0;

    for (size_t pos = 0; pos < buf_size;) { // parser
        char spring[MAX_SPRINGS];
        int spring_count = 0;

        int size[MAX_GROUPS];
        int size_count = 0;

        while (buf[pos] != ' ') { spring[spring_count++] = buf[pos++]; }
        pos++;                     // whitespace
        while (buf[pos] != '\n') { // groups
            i64 value = aoc_parse_nonnegative(buf, &pos);
            assert(value > 0);
            size[size_count++] = value;
            if (buf[pos] == ',') pos++;
        }

        spring[spring_count] = '\0'; // for printing

        { // part 1
            ust_CacheItem cache = ust_CacheItem_init(CacheItem_hash, CacheItem_equal);
            size_t count = find_solutions(&cache, spring, spring_count, size, size_count, 0);
            part1 += count;
            ust_CacheItem_free(&cache);
        }

        { // part 2
            ust_CacheItem cache = ust_CacheItem_init(CacheItem_hash, CacheItem_equal);
            char big_spring[MAX_SPRINGS];
            int big_spring_count = 0;
            for (int i = 1; i <= 5; i++) {
                for (int j = 0; j < spring_count; j++) { big_spring[big_spring_count++] = spring[j]; }
                if (i != 5) { big_spring[big_spring_count++] = '?'; }
            }
            assert(big_spring_count == 5 * spring_count + 4);

            int big_size[MAX_GROUPS];
            int big_size_count = 0;
            for (int i = 1; i <= 5; i++) {
                for (int j = 0; j < size_count; j++) { big_size[big_size_count++] = size[j]; }
            }
            assert(big_size_count == 5 * size_count);
            big_spring[big_spring_count] = '\0'; // for printing

            size_t count = find_solutions(&cache, big_spring, big_spring_count, big_size, big_size_count, 0);
            part2 += count;

            ust_CacheItem_free(&cache);
        }

        pos++;
    }

    snprintf(result->part1, sizeof(result->part1), "%ld", part1);
    snprintf(result->part2, sizeof(result->part2), "%ld", part2);
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
