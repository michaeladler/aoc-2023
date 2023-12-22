/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#include <slice99.h>
#include <xxhash.h>

#define MAX_RULES 8

typedef enum { LT, GT, JUMP } rule_kind_e;

typedef struct {
    rule_kind_e kind;
    char variable;
    int value;
    CharSlice99 destination;
} rule_t;

typedef struct {
    CharSlice99 name;
    rule_t rule[MAX_RULES];
    int rule_count;
} workflow_t;

typedef struct {
    int x, m, a, s;
} data_t;

#define P
#define T workflow_t
#include <ust.h>

static size_t workflow_t_hash(workflow_t *wf) { return (size_t)XXH3_64bits(wf->name.ptr, wf->name.len); }

static int workflow_t_equal(workflow_t *lhs, workflow_t *rhs) { return CharSlice99_primitive_eq(lhs->name, rhs->name); }

static inline bool rule_matches(rule_t *self, data_t data) {
    if (self->kind == JUMP) return true;
    int data_value;
    switch (self->variable) {
    case 'x': data_value = data.x; break;
    case 'm': data_value = data.m; break;
    case 'a': data_value = data.a; break;
    case 's': data_value = data.s; break;
    }
    if (self->kind == LT) { return data_value < self->value; }
    return data_value > self->value;
}

static inline CharSlice99 *workflow_next(workflow_t *wf, data_t data) {
    for (int i = 0; i < wf->rule_count; i++) {
        if (rule_matches(&wf->rule[i], data)) { return &wf->rule[i].destination; }
    }
    return NULL;
}

void solve(char *buf, size_t buf_size, Solution *result) {
    int part1 = 0, part2 = 0;
    size_t pos = 0;

    _cleanup_(ust_workflow_t_free) ust_workflow_t workflows = ust_workflow_t_init(workflow_t_hash, workflow_t_equal);
    ust_workflow_t_reserve(&workflows, 128);

    while (1) {
        workflow_t wf = {.rule_count = 0};

        size_t start = pos;
        while (buf[pos] != '{') pos++;
        wf.name = CharSlice99_new(&buf[start], pos - start);
        pos++;

        // next are the rules
        while (1) {
            char op = buf[pos + 1]; // look ahead
            rule_t *rule = &wf.rule[wf.rule_count];
            if (op == '<' || op == '>') {
                rule->kind = op == '<' ? LT : GT;
                rule->variable = buf[pos];
                pos += 2;
                rule->value = aoc_parse_nonnegative(buf, &pos);
                pos++;
                size_t start = pos;
                while (buf[pos] != ',') pos++;
                rule->destination = CharSlice99_new(&buf[start], pos - start);
                pos++;
                wf.rule_count++;
            } else { // this is the last rule, a jump
                rule->kind = JUMP;
                size_t start = pos;
                while (buf[pos] != '}') pos++;
                rule->destination = CharSlice99_new(&buf[start], pos - start);
                wf.rule_count++;
                break;
            }
        }

        log_debug(">> workflow %.*s has %d rules", wf.name.len, wf.name.ptr, wf.rule_count);
        ust_workflow_t_insert(&workflows, wf);

        aoc_parse_seek(buf, &pos, '\n');
        pos++;
        if (buf[pos] == '\n') break; // workflows finished, inputs are next
    }

    ust_workflow_t_node *start_node = ust_workflow_t_find(&workflows, (workflow_t){.name = CharSlice99_from_str("in")});

    CharSlice99 accepted = CharSlice99_from_str("A"), rejected = CharSlice99_from_str("R");

    pos++;
    while (pos < buf_size) {
        if (buf[pos++] == '{') {
            data_t data = {0};
            while (pos < buf_size) {
                char var = buf[pos];
                pos += 2;
                int value = aoc_parse_nonnegative(buf, &pos);
                switch (var) {
                case 'x': data.x = value; break;
                case 'm': data.m = value; break;
                case 'a': data.a = value; break;
                case 's': data.s = value; break;
                }
                if (buf[pos] == '}') break;
                pos++;
            }
            log_debug(">> input: x=%d, m=%d, a=%d, s=%d", data.x, data.m, data.a, data.s);

            workflow_t *current = &start_node->key;
            while (1) {
                CharSlice99 next = *workflow_next(current, data);
                log_debug("%.*s -> %.*s", current->name.len, current->name.ptr, next.len, next.ptr);
                if (CharSlice99_primitive_eq(next, accepted)) {
                    part1 += data.x + data.a + data.m + data.s;
                    break;
                } else if (CharSlice99_primitive_eq(next, rejected)) {
                    break;
                }
                current = &ust_workflow_t_find(&workflows, (workflow_t){.name = next})->key;
            }
        }
    }

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
