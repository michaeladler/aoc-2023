/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

typedef struct {
    char label[3];
} Node;

typedef struct {
    u32 left_idx;
    u32 right_idx;
} Waypoint;

static inline u32 Node_to_idx(Node n) {
    int c0 = n.label[0] - 'A';
    int c1 = n.label[1] - 'A';
    int c2 = n.label[2] - 'A';
    assert(c0 >= 0 && c1 >= 0 && c2 >= 0);
    assert(c0 < 26 && c1 < 26 && c2 < 26);
    return c0 + 26 * c1 + 26 * 26 * c2;
}

static inline Node idx_to_Node(u32 idx) {
    Node result;
    for (int i = 0; i < 3; i++) {
        result.label[i] = (idx % 26) + 'A';
        idx /= 26;
    }
    assert(idx == 0);
    return result;
}

void solve(const char *buf, size_t buf_size, Solution *result) {
    int part1 = 0, part2 = 0;

    char instruction[300];
    int instruction_count = 0;
    Waypoint node_to_wp[26 * 26 * 26] = {0};

    { // parser
        size_t pos = 0;
        while (buf[pos] != '\n') {
            instruction[instruction_count++] = buf[pos++];
        }
        pos += 2; // two newlines
        log_debug("instructions: %.*s", instruction_count, instruction);
        while (pos < buf_size) {
            Node tmp;
            for (int i = 0; i < 3; i++) { tmp.label[i] = buf[pos++]; }
            pos += 4;

            Waypoint *wp = &node_to_wp[Node_to_idx(tmp)];

            for (int i = 0; i < 3; i++) { tmp.label[i] = buf[pos++]; }
            wp->left_idx = Node_to_idx(tmp);

            pos += 2;
            for (int i = 0; i < 3; i++) { tmp.label[i] = buf[pos++]; }
            wp->right_idx = Node_to_idx(tmp);

            pos += 2;
        }
    }
    assert(instruction_count > 0);

    // part 1
    Node start = {.label = {'A', 'A', 'A'}};
    Node dest = {.label = {'Z', 'Z', 'Z'}};
    size_t dest_idx = Node_to_idx(dest);

    size_t current = Node_to_idx(start);
    size_t instruction_idx = 0;
    while (current != dest_idx) {
        Waypoint wp = node_to_wp[current];
        char inst = instruction[instruction_idx];
        current = inst == 'L' ? wp.left_idx : wp.right_idx;
        instruction_idx = (instruction_idx + 1) % instruction_count;
        part1++;
    }

    aoc_itoa(part1, result->part1, 10);
    aoc_itoa(part2, result->part2, 10);
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
