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
#define MAX_NODES 1024
#define MAX_NEIGHBORS 8
#define LOWER 1
#define UPPER 4000

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

typedef struct {
    int lower, upper; // inclusive
} interval_t;

typedef struct {
    interval_t x, m, a, s;
} constraint_t;

typedef struct {
    constraint_t constraint;
    CharSlice99 src;
    int rule_idx;
} state_t;

static size_t workflow_t_hash(workflow_t *wf) { return (size_t)XXH3_64bits(wf->name.ptr, wf->name.len); }

static int workflow_t_equal(workflow_t *lhs, workflow_t *rhs) { return CharSlice99_primitive_eq(lhs->name, rhs->name); }

static inline bool rule_matches(rule_t *self, data_t data) {
    if (self->kind == JUMP) return true;
    int data_value = 0;
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

static inline void interval_apply_rule(interval_t *interval, rule_t *rule) {
    if (rule->kind == LT) {
        interval->upper = rule->value - 1; // interval.upper < rule->value
    } else if (rule->kind == GT) {
        interval->lower = rule->value + 1; // interval.lower > rule->value
    }
}

static inline void interval_negate_rule(interval_t *interval, rule_t *rule) {
    if (rule->kind == LT) {
        interval->lower = rule->value; // interval.lower >= rule->value
    } else if (rule->kind == GT) {
        interval->upper = rule->value; // interval.upper <= rule->value
    }
}

static inline i64 interval_cardinality(interval_t interval) {
    if (interval.upper < interval.lower) return 0;
    return interval.upper - interval.lower + 1;
}

static inline i64 constraint_cardinality(constraint_t c) {
    return interval_cardinality(c.x) * interval_cardinality(c.m) * interval_cardinality(c.a) *
           interval_cardinality(c.s);
}

static inline constraint_t constraint_apply_rule(constraint_t constraint, rule_t *rule) {
    constraint_t result = constraint;
    switch (rule->variable) {
    case 'x': interval_apply_rule(&result.x, rule); break;
    case 'm': interval_apply_rule(&result.m, rule); break;
    case 'a': interval_apply_rule(&result.a, rule); break;
    case 's': interval_apply_rule(&result.s, rule); break;
    }
    return result;
}

static inline constraint_t constraint_negate_rule(constraint_t constraint, rule_t *rule) {
    constraint_t result = constraint;
    switch (rule->variable) {
    case 'x': interval_negate_rule(&result.x, rule); break;
    case 'm': interval_negate_rule(&result.m, rule); break;
    case 'a': interval_negate_rule(&result.a, rule); break;
    case 's': interval_negate_rule(&result.s, rule); break;
    }
    return result;
}

void solve(char *buf, size_t buf_size, Solution *result) {
    int part1 = 0;
    size_t pos = 0;

    _cleanup_(ust_workflow_t_free) ust_workflow_t workflows = ust_workflow_t_init(workflow_t_hash, workflow_t_equal);
    ust_workflow_t_reserve(&workflows, 256);

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

    CharSlice99 start = CharSlice99_from_str("in");
    ust_workflow_t_node *start_node = ust_workflow_t_find(&workflows, (workflow_t){.name = start});
    CharSlice99 accepted = CharSlice99_from_str("A"), rejected = CharSlice99_from_str("R");

    // part 1
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
            workflow_t *current = &start_node->key;
            while (1) {
                CharSlice99 *next = workflow_next(current, data);
                assert(next != NULL);
                if (CharSlice99_primitive_eq(*next, accepted)) {
                    part1 += data.x + data.a + data.m + data.s;
                    break;
                } else if (CharSlice99_primitive_eq(*next, rejected)) {
                    break;
                }
                current = &ust_workflow_t_find(&workflows, (workflow_t){.name = *next})->key;
            }
        }
    }

    // part 2
    i64 part2 = 0;

    state_t stack[4096];
    interval_t initial = {.lower = LOWER, .upper = UPPER};
    stack[0] = (state_t){.src = start,
                         .constraint = (constraint_t){.x = initial, .m = initial, .a = initial, .s = initial},
                         .rule_idx = 0};
    int stack_size = 1;
    while (stack_size != 0) {
        state_t state = stack[--stack_size]; // pop
        if (CharSlice99_primitive_eq(state.src, accepted)) {
            part2 += constraint_cardinality(state.constraint);
        } else if (CharSlice99_primitive_eq(state.src, rejected)) {
            continue;
        } else {
            ust_workflow_t_node *node = ust_workflow_t_find(&workflows, (workflow_t){.name = state.src});
            assert(node != NULL);
            workflow_t *wf = &node->key;
            rule_t *rule = &wf->rule[state.rule_idx];
            constraint_t yes = constraint_apply_rule(state.constraint, rule);
            constraint_t no = constraint_negate_rule(state.constraint, rule);
            stack[stack_size++] = (state_t){.src = rule->destination, .rule_idx = 0, .constraint = yes};
            if (rule->kind != JUMP) {
                stack[stack_size++] = (state_t){.src = state.src, .rule_idx = state.rule_idx + 1, .constraint = no};
            }
        }
    }

    snprintf(result->part1, sizeof(result->part1), "%d", part1);
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
