#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "aoc/macros.h"
#include "backtrack.h"
#include "log.h"

#define MAXCANDIDATES 2 /* max possible next extensions */
#define NMAX 32         /* maximum solution size */

static bool finished = false; /* found all solutions yet? */

/* Check if a[0], ... a[k] is a solution */
static inline bool is_a_solution(int _unused_ a[], int k, data ctx) {
    if (k + 1 != ctx->idx_unknown_count) {
        // there are still some '?' unset
        return false;
    }

    int actual_damage = spring_count_damaged(ctx->spring); // how many we know for sure are damaged

    int expected_damage = 0;
    for (int i = 0; i < ctx->group_count; i++) expected_damage += ctx->group[i];

    if (actual_damage != expected_damage) return false;
    log_debug("actual damage matches expected damage, checking groups");

    {
        char buf[33] = {0};
        for (int i = 0; i < ctx->spring.len; i++) {
            buf[i] = spring_is_damaged(&ctx->spring, i) ? DAMAGED : OPERATIONAL;
        }
        log_debug("checking potential solution: %s", buf);
    }

    int actual_group[MAX_GROUPS];
    int actual_group_count = 0;

    // count actual groups and check if it maches with the expectation
    for (int pos = 0; pos < ctx->spring.len;) {
        while (pos < ctx->spring.len && !spring_is_damaged(&ctx->spring, pos)) pos++; // seek to first damaged
        int start = pos;                                                              // pos is damaged
        while (pos < ctx->spring.len && spring_is_damaged(&ctx->spring, pos)) pos++;
        // now pos is at undamaged
        int count = pos - start;
        if (count > 0) {
            log_debug("found group of damaged of size %d", count);
            actual_group[actual_group_count++] = count;
        }
    }
    log_debug("found %d groups", actual_group_count);

    if (actual_group_count != ctx->group_count) {
        log_debug("group count does not match! actual %d, expected %d", actual_group_count, ctx->group_count);
        return false;
    }
    for (int i = 0; i < ctx->group_count; i++) {
        if (ctx->group[i] != actual_group[i]) {
            log_debug("group of wrong size");
            return false;
        }
    }

    return true;
}

static inline void process_solution(int a[], int k, data ctx) {
    ctx->solution_count++;

    char buf[33] = {0};
    for (int i = 0; i < ctx->spring.len; i++) { buf[i] = spring_is_damaged(&ctx->spring, i) ? DAMAGED : OPERATIONAL; }
    log_debug("!!! found solution: %s", buf);
}

/* What are possible candidates for position k?
 * The candidates are stored in c[] and its length is stored in ncandidates.
 */
static inline void construct_candidates(int a[], int k, data ctx, int c[], int *ncandidates) {
    if (k >= ctx->idx_unknown_count) {
        *ncandidates = 0;
        return;
    }

    c[0] = 0; // OPERATIONAL;
    c[1] = 1; // DEAD
    *ncandidates = 2;
}

static inline void make_move(int a[], int k, data input) {
    assert(k >= 0);
    int idx = input->idx_unknown[k];
    if (a[k] == 1) {
        spring_set_damaged(&input->spring, idx);
    } else {
        spring_unset_damaged(&input->spring, idx);
    }
}

static inline void unmake_move(int a[], int k, data input) {
    int idx = input->idx_unknown[k];
    spring_unset_damaged(&input->spring, idx);
}

/**
 * Copyright 2003 by Steven S. Skiena; all rights reserved.
 *
 * Permission is granted for use in non-commerical applications
 * provided this copyright notice remains intact and unchanged.
 *
 * This program appears in my book:
 *
 * "Programming Challenges: The Programming Contest Training Manual"
 * by Steven Skiena and Miguel Revilla, Springer-Verlag, New York 2003.
 *
 * See our website www.programming-challenges.com for additional information.
 *
 * This book can be ordered from Amazon.com at
 *
 * http://www.amazon.com/exec/obidos/ASIN/0387001638/thealgorithmrepo/
 *
 */
void backtrack(int a[], int k, data input) {
    int c[MAXCANDIDATES]; /* candidates for next position */
    int ncandidates;      /* next position candidate count */
    int i;                /* counter */

    if (is_a_solution(a, k, input)) {
        process_solution(a, k, input);
    } else {
        k = k + 1;
        construct_candidates(a, k, input, c, &ncandidates);
        for (i = 0; i < ncandidates; i++) {
            log_debug("checking candidate %d for k=%d (idx=%d)", c[i], k, input->idx_unknown[k]);
            a[k] = c[i];
            make_move(a, k, input);
            backtrack(a, k, input);
            unmake_move(a, k, input);
            if (finished) return; /* terminate early */
        }
    }
}
