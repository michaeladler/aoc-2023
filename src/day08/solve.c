/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#define MAX_NODES 1024

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
    char instruction[300];
    u32 instruction_count = 0;
    Waypoint node_to_wp[26 * 26 * 26] = {0};

    Node nodes[MAX_NODES];
    u32 node_count = 0;

    { // parser
        size_t pos = 0;
        while (buf[pos] != '\n') {
            instruction[instruction_count++] = buf[pos++];
        }
        pos += 2; // two newlines
        log_debug("instructions: %.*s", instruction_count, instruction);
        while (pos < buf_size) {
            Node n;
            for (int i = 0; i < 3; i++) { n.label[i] = buf[pos++]; }
            pos += 4;
            nodes[node_count++] = n;

            Waypoint *wp = &node_to_wp[Node_to_idx(n)];

            for (int i = 0; i < 3; i++) { n.label[i] = buf[pos++]; }
            wp->left_idx = Node_to_idx(n);

            pos += 2;
            for (int i = 0; i < 3; i++) { n.label[i] = buf[pos++]; }
            wp->right_idx = Node_to_idx(n);

            pos += 2;
        }
    }
    assert(instruction_count > 0);
    assert(node_count <= MAX_NODES);
    log_debug("node_count: %d", node_count);

    int part1 = 0;
    { // part 1
        Node start = {.label = {'A', 'A', 'A'}};
        Node dest = {.label = {'Z', 'Z', 'Z'}};
        u32 dest_idx = Node_to_idx(dest);

        u32 current = Node_to_idx(start);
        u32 instruction_idx = 0;
        while (current != dest_idx) {
            Waypoint wp = node_to_wp[current];
            char inst = instruction[instruction_idx];
            current = inst == 'L' ? wp.left_idx : wp.right_idx;
            instruction_idx = (instruction_idx + 1) % instruction_count;
            part1++;
        }
    }
    aoc_itoa(part1, result->part1, 10);

    // part 2: find cycles, compute lcm
    i64 part2 = 1;
    for (u32 i = 0; i < node_count; i++) {
        Node start = nodes[i];
        if (start.label[2] != 'A') continue;
        log_debug("checking starting node %.*s", 3, start.label);
        u32 steps = 0;
        u32 current = Node_to_idx(start);
        while (idx_to_Node(current).label[2] != 'Z') {
            Waypoint wp = node_to_wp[current];
            char inst = instruction[steps % instruction_count];
            current = inst == 'L' ? wp.left_idx : wp.right_idx;
            steps++;
        }
        log_debug("steps to first z node: %d", steps);
        part2 = aoc_lcm(part2, steps);
    }
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
