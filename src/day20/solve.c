/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"
#include "slice99.h"

#include <xxh3.h>

#define MAX_NAME_LEN 4
#define MAX_OUTPUTS 8
#define MAX_CONJUNCTION_ITEMS 32

typedef enum { BROADCAST, FLIP_FLOP, CONJUNCTION } kind_e;

typedef enum { PULSE_LOW, PULSE_HIGH } pulse_e;

typedef struct {
    CharSlice99 name;
    pulse_e pulse;
} conjunction_item_t;

typedef struct {
    conjunction_item_t items[MAX_CONJUNCTION_ITEMS];
    int len;
} conjunction_item_list;

typedef union {
    bool on;
    conjunction_item_list list;
} module_state_t;

typedef struct {
    CharSlice99 name;
    kind_e kind;
    module_state_t state;
    CharSlice99 output[MAX_OUTPUTS];
    int output_count;
} module_t;

#define P
#define T module_t
#include <ust.h>

typedef struct {
    CharSlice99 source, destination;
    pulse_e input;
} state_t;

#define P
#define T state_t
#include <que.h>

static size_t module_t_hash(module_t *item) { return XXH3_64bits(item->name.ptr, item->name.len); }
static int module_t_equal(module_t *lhs, module_t *rhs) { return CharSlice99_primitive_eq(lhs->name, rhs->name); }

void solve(char *buf, size_t buf_size, Solution *result) {
    i64 part1 = 0, part2 = 0;

    _cleanup_(ust_module_t_free) ust_module_t modules = ust_module_t_init(module_t_hash, module_t_equal);

    size_t pos = 0;
    while (pos < buf_size) {
        char c = buf[pos];

        module_t m = {.kind = BROADCAST, .output_count = 0};
        if (c == '%') {
            m.kind = FLIP_FLOP;
            m.state.on = false;
            pos++;
        } else if (c == '&') {
            m.kind = CONJUNCTION;
            pos++;
        }
        int len = 0;
        while (buf[pos + len] != ' ') { len++; }
        m.name = CharSlice99_new(&buf[pos], len);
        pos += len;
        log_debug(">> module name: %.*s", m.name.len, m.name.ptr);

        aoc_parse_seek(buf, &pos, '>');
        pos += 2;

        while (1) {
            len = 0;
            while (buf[pos + len] != ',' && buf[pos + len] != '\n') { len++; }
            m.output[m.output_count++] = CharSlice99_new(&buf[pos], len);
            pos += len;
            if (buf[pos] == '\n') { break; }
            pos += 2; // skip comma
        }
        for (int i = 0; i < m.output_count; i++) { log_debug("\toutput: %.*s", m.output[i].len, m.output[i].ptr); }

        aoc_parse_seek(buf, &pos, '\n');
        pos++;

        ust_module_t_insert(&modules, m);
    }

    // initialize conjunction items with their incoming signals to low
    foreach (ust_module_t, &modules, it) {
        module_t *m = &it.node->key;
        CharSlice99 source = m->name;
        for (int i = 0; i < m->output_count; i++) {
            CharSlice99 destination = m->output[i];
            ust_module_t_node *node = ust_module_t_find(&modules, (module_t){.name = destination});
            if (node == NULL) continue;
            module_t *dest_module = &node->key;
            if (dest_module->kind == CONJUNCTION) {
                bool found = false;
                for (int j = 0; j < dest_module->state.list.len; j++) {
                    if (CharSlice99_primitive_eq(dest_module->state.list.items[j].name, source)) {
                        found = true;
                        dest_module->state.list.items[j].pulse = PULSE_LOW;
                        break;
                    }
                }
                if (!found) {
                    log_debug("%.*s is an input to %.*s", source.len, source.ptr, destination.len, destination.ptr);
                    int len = dest_module->state.list.len;
                    dest_module->state.list.items[len] = (conjunction_item_t){.name = source, .pulse = PULSE_LOW};
                    dest_module->state.list.len = len + 1;
                }
            }
        }
    }

    _cleanup_(que_state_t_free) que_state_t queue = que_state_t_init();
    i64 high_count = 0, low_count = 0;
    for (int round = 1; round <= 1000; round++) {
        que_state_t_push(&queue, (state_t){.source = CharSlice99_from_str("button"),
                                           .destination = CharSlice99_from_str("broadcaster"),
                                           .input = PULSE_LOW});
        while (!que_state_t_empty(&queue)) {
            state_t *current = que_state_t_front(&queue);
            log_debug("%.*s -%c-> %.*s", current->source.len, current->source.ptr,
                      current->input == PULSE_HIGH ? 'H' : 'L', current->destination.len, current->destination.ptr);
            if (current->input == PULSE_HIGH) {
                high_count++;
            } else {
                low_count++;
            }

            ust_module_t_node *node = ust_module_t_find(&modules, (module_t){.name = current->destination});
            if (node) {
                module_t *m = &node->key;
                switch (m->kind) {
                case BROADCAST:
                    for (int i = 0; i < m->output_count; i++) {
                        state_t item = {
                            .source = current->destination,
                            .destination = m->output[i],
                            .input = current->input,
                        };
                        que_state_t_push(&queue, item);
                    }
                    break;
                case FLIP_FLOP:
                    if (current->input == PULSE_LOW) {
                        m->state.on = !m->state.on;
                        for (int i = 0; i < m->output_count; i++) {
                            state_t item = {
                                .source = current->destination,
                                .destination = m->output[i],
                                .input = m->state.on ? PULSE_HIGH : PULSE_LOW,
                            };
                            que_state_t_push(&queue, item);
                        }
                    }
                    break;
                case CONJUNCTION:
                    // the conjunction module first updates its memory for that input
                    for (int i = 0; i < m->state.list.len; i++) {
                        conjunction_item_t *item = &m->state.list.items[i];
                        if (CharSlice99_primitive_eq(item->name, current->source)) {
                            item->pulse = current->input;
                            break;
                        }
                    }
                    // if high pulses for all inputs, it sends a low pulse; otherwise, it sends a high pulse
                    pulse_e pulse = PULSE_LOW;
                    for (int i = 0; i < m->state.list.len; i++) {
                        conjunction_item_t *item = &m->state.list.items[i];
                        if (item->pulse == PULSE_LOW) {
                            pulse = PULSE_HIGH;
                            break;
                        }
                    }
                    for (int i = 0; i < m->output_count; i++) {
                        state_t item = {
                            .source = current->destination,
                            .destination = m->output[i],
                            .input = pulse,
                        };
                        que_state_t_push(&queue, item);
                    }
                    break;
                }
            }
            que_state_t_pop(&queue);
        }
        log_debug("round %d: low_count: %ld, high_count: %ld", round, low_count, high_count);
    }
    part1 += high_count * low_count;

    snprintf(result->part1, sizeof(result->part1), "%ld", part1);
    snprintf(result->part2, sizeof(result->part2), "%ld", part2);
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
