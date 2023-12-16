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

#define MAX_LABEL_LEN 8
#define MAX_LENSES_PER_BOX 512
#define BOX_COUNT 256

typedef struct {
    CharSlice99 label;
    int focal;
} lens_t;

typedef struct {
    lens_t item[MAX_LENSES_PER_BOX];
    int item_count;
} box_t;

typedef struct {
    box_t box[BOX_COUNT];
} hashmap_t;

static inline u32 hash(CharSlice99 s) {
    u32 value = 0;
    for (size_t i = 0; i < s.len; i++) { value = ((value + *CharSlice99_get(s, i)) * 17) % BOX_COUNT; }
    return value;
}

static inline void hashmap_upsert(hashmap_t *self, CharSlice99 label, int focal) {
    u32 box_id = hash(label);
    box_t *box = &self->box[box_id];
    // check if we have to update an existing value
    for (int i = 0; i < box->item_count; i++) {
        if (CharSlice99_primitive_eq(box->item[i].label, label)) {
            log_debug(">> updating box %d: label %.*s has new value %d", box_id, label.len, label.ptr, focal);
            box->item[i].focal = focal;
            return;
        }
    }
    // insert new value
    log_debug(">> appending to box %d: label %.*s with value %d", box_id, label.len, label.ptr, focal);
    box->item_count++;
    lens_t lens = {.label = label, .focal = focal};
    box->item[box->item_count - 1] = lens;
}

static inline void hashmap_remove(hashmap_t *self, CharSlice99 label) {
    u32 box_id = hash(label);
    box_t *box = &self->box[box_id];
    if (box->item_count == 0) return;
    // find index of label in box
    int idx = 0;
    while (idx < box->item_count) {
        if (CharSlice99_primitive_eq(box->item[idx].label, label)) break;
        idx++;
    }
    if (idx == box->item_count) {
        log_debug(">> box %d: label %.*s not found in hashmap, nothing to do", box_id, label.len, label.ptr);
        return;
    }
    log_debug(">> box %d: found label %.*s at position %idx, removing it", box_id, label.len, label.ptr);
    // shift neighbors
    for (int i = idx + 1; i < box->item_count; i++) { box->item[i - 1] = box->item[i]; }
    box->item_count--;
}

static inline void apply_instruction(hashmap_t *hm, CharSlice99 instruction) {
    if (*CharSlice99_last(instruction) == '-') {
        hashmap_remove(hm, CharSlice99_sub(instruction, 0, instruction.len - 1));
    } else {
        int i = 0;
        while (*CharSlice99_get(instruction, i) != '=') i++;
        CharSlice99 label = CharSlice99_sub(instruction, 0, i);
        size_t pos = i + 1;
        i64 focal = aoc_parse_nonnegative(instruction.ptr, &pos);
        hashmap_upsert(hm, label, focal);
    }
}

static inline u64 compute_focusing_power(hashmap_t *hm) {
    u64 result = 0;
    for (int box_number = 0; box_number < BOX_COUNT; box_number++) {
        box_t *box = &hm->box[box_number];
        for (int j = 0; j < box->item_count; j++) { result += (1 + box_number) * (1 + j) * box->item[j].focal; }
    }
    return result;
}

void solve(char *buf, size_t buf_size, Solution *result) {
    u64 part1 = 0, part2 = 0;
    CharSlice99 input = CharSlice99_from_ptrdiff(buf, &buf[buf_size - 2]); // no '\0', no '\n'

    hashmap_t hm = {0};

    size_t start = 0;
    size_t pos = 0;
    while (pos < input.len) {
        if (*CharSlice99_get(input, pos) == ',') {
            CharSlice99 s = CharSlice99_sub(input, start, pos);
            part1 += hash(s);
            apply_instruction(&hm, s);
            start = pos + 1;
        }
        pos++;
    }
    CharSlice99 s = CharSlice99_sub(input, start, pos + 1);
    part1 += hash(s);
    apply_instruction(&hm, s);
    part2 = compute_focusing_power(&hm);

    snprintf(result->part1, sizeof(result->part1), "%lu", part1);
    snprintf(result->part2, sizeof(result->part2), "%lu", part2);
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
