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
    CharSlice99 name;
    int id;
} name_id_t;

#define P
#define T name_id_t
#include <ust.h>

typedef struct {
    int neighbor[MAX_NEIGHBORS];
    int neighbor_count;
} adj_list_t;

typedef struct {
    int n;
    adj_list_t adj[MAX_NODES];
} graph_t;

typedef struct {
    int lower; // inclusive
    int upper; // inclusive
} interval_t;

typedef struct {
    interval_t x, m, a, s;
} constraint_t;

static size_t name_hash(name_id_t *item) { return (size_t)XXH3_64bits(item->name.ptr, item->name.len); }

static int name_equal(name_id_t *lhs, name_id_t *rhs) { return CharSlice99_primitive_eq(lhs->name, rhs->name); }

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

static inline void interval_apply_rule(interval_t *interval, rule_t *rule) {
    if (rule->kind == LT) {
        interval->upper = MIN(rule->value - 1, interval->upper); // interval.upper < rule->value
    } else if (rule->kind == GT) {
        interval->lower = MAX(rule->value + 1, interval->lower); // interval.lower > rule->value
    }
}

static inline void interval_negate_rule(interval_t *interval, rule_t *rule) {
    if (rule->kind == LT) {
        interval->upper = MAX(rule->value, interval->upper); // interval.upper >= rule->value
    } else if (rule->kind == GT) {
        interval->lower = MIN(rule->value, interval->lower); // interval.lower <= rule->value
    }
}

static inline constraint_t constraint_apply_rule(constraint_t constraint, rule_t *r) {
    constraint_t result = constraint;
    switch (r->variable) {
    case 'x': interval_apply_rule(&result.x, r); break;
    case 'm': interval_apply_rule(&result.m, r); break;
    case 'a': interval_apply_rule(&result.a, r); break;
    case 's': interval_apply_rule(&result.s, r); break;
    }
    return result;
}

static inline constraint_t constraint_negate_rule(constraint_t constraint, rule_t *r) {
    constraint_t result = constraint;
    switch (r->variable) {
    case 'x': interval_negate_rule(&result.x, r); break;
    case 'm': interval_negate_rule(&result.m, r); break;
    case 'a': interval_negate_rule(&result.a, r); break;
    case 's': interval_negate_rule(&result.s, r); break;
    }
    return result;
}

static void find_all_paths(graph_t *graph, int current, int destination, bool visited[], int path[], int *path_idx,
                           ust_workflow_t *workflows, CharSlice99 *id_to_name) {
    // mark the current node and store it in path[]
    visited[current] = true;
    path[*path_idx] = current;
    *path_idx = *path_idx + 1;

    if (current == destination) {
        log_debug(">> found new path to destination:");
        for (int i = 0; i < *path_idx; i++) {
            int id = path[i];
            CharSlice99 name = id_to_name[id];
            log_debug("%.*s", name.len, name.ptr);
        }
    } else {
        adj_list_t *lst = &graph->adj[current];
        for (int i = 0; i < lst->neighbor_count; i++) {
            int neighbor_id = lst->neighbor[i];
            if (!visited[neighbor_id]) {
                find_all_paths(graph, neighbor_id, destination, visited, path, path_idx, workflows, id_to_name);
            }
        }
    }

    // Remove current vertex from path[] and mark it as
    // unvisited
    *path_idx = *path_idx - 1;
    visited[current] = false;
}

void solve(char *buf, size_t buf_size, Solution *result) {
    int part1 = 0, part2 = 0;
    size_t pos = 0;

    _cleanup_(ust_workflow_t_free) ust_workflow_t workflows = ust_workflow_t_init(workflow_t_hash, workflow_t_equal);
    ust_workflow_t_reserve(&workflows, 256);

    int id_count = 0;
    _cleanup_(ust_name_id_t_free) ust_name_id_t name_to_id = ust_name_id_t_init(name_hash, name_equal);
    ust_name_id_t_reserve(&name_to_id, 256);
    CharSlice99 id_to_name[MAX_NODES];

    while (1) {
        workflow_t wf = {.rule_count = 0};

        size_t start = pos;
        while (buf[pos] != '{') pos++;
        wf.name = CharSlice99_new(&buf[start], pos - start);

        ust_name_id_t_insert(&name_to_id, (name_id_t){.id = id_count, .name = wf.name});
        log_debug("id_count: %d", id_count);
        id_to_name[id_count++] = wf.name;

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

    ust_name_id_t_insert(&name_to_id, (name_id_t){.id = id_count, .name = accepted});
    id_to_name[id_count++] = accepted;
    ust_name_id_t_insert(&name_to_id, (name_id_t){.id = id_count, .name = rejected});
    id_to_name[id_count++] = rejected;

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

    // part 2
    graph_t graph = {.n = id_count};
    foreach (ust_workflow_t, &workflows, it) { // build up graph
        int id = ust_name_id_t_find(&name_to_id, (name_id_t){.name = it.node->key.name})->key.id;
        workflow_t *wf = &it.node->key;
        adj_list_t *lst = &graph.adj[id];
        lst->neighbor_count = 0;

        for (int i = 0; i < wf->rule_count; i++) {
            rule_t *r = &wf->rule[i];
            ust_name_id_t_node *node = ust_name_id_t_find(&name_to_id, (name_id_t){.name = r->destination});
            assert(node != NULL);
            int neighbor_id = node->key.id;
            lst->neighbor[lst->neighbor_count++] = neighbor_id;
        }
    };

    int start_id = ust_name_id_t_find(&name_to_id, (name_id_t){.name = start})->key.id;
    int dest_id = ust_name_id_t_find(&name_to_id, (name_id_t){.name = accepted})->key.id;
    bool visited[MAX_NODES];
    for (size_t i = 0; i < sizeof(visited); i++) visited[i] = false;
    int path[MAX_NODES], path_idx = 0;
    find_all_paths(&graph, start_id, dest_id, visited, path, &path_idx, &workflows, id_to_name);

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
