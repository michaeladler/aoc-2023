/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#define GRID_SIZE 256
#define INFINITY INT_MAX

typedef enum { NORTH, EAST, SOUTH, WEST } direction_t;

typedef struct {
    Point2D position;
    direction_t direction; // current direction
    int direction_count;   // how many times we went into the current direction
} Node;

typedef struct {
    Node node;
    int dist;
} State;

#define P
#define T State
#include <pqu.h>

typedef struct {
    Node node;    // key
    int distance; // value
} NodeDistance;

#define P
#define T NodeDistance
#include <ust.h>

// min queue
static int State_compare(State *lhs, State *rhs) { return lhs->dist < rhs->dist; }

static size_t Node_hash(Node *node) {
    const size_t prime = 31;
    size_t hash = 17;
    // Combine the hash of each struct member
    hash = hash * prime + (size_t)node->position.x;
    hash = hash * prime + (size_t)node->position.y;
    hash = hash * prime + (size_t)node->direction;
    hash = hash * prime + (size_t)node->direction_count;
    return hash;
}
static int Node_equal(Node *lhs, Node *rhs) {
    return Point2D_equal(&lhs->position, &rhs->position) && lhs->direction == rhs->direction &&
           lhs->direction_count == rhs->direction_count;
}

static size_t NodeDistance_hash(NodeDistance *item) { return Node_hash(&item->node); }

static int NodeDistance_equal(NodeDistance *lhs, NodeDistance *rhs) { return Node_equal(&lhs->node, &rhs->node); }

static inline void add_node(int rows, int cols, Node node, Node *arr, int *arr_len) {
    if (node.position.x < 0 || node.position.x >= cols || node.position.y < 0 || node.position.y >= rows) { return; }
    int count = *arr_len;
    arr[count++] = node;
    *arr_len = count;
}

static inline int find_neighbors(int rows, int cols, Node current, Node *neighbor) {
    bool must_turn = current.direction_count == 3;
    log_debug(">> visiting %d,%d, must_turn: %d, dir: %d, dir_count: %d", current.position.y, current.position.x,
              must_turn, current.direction, current.direction_count);

    int count = 0;
    Node north = {.position = (Point2D){.x = current.position.x, .y = current.position.y - 1},
                  .direction = NORTH,
                  .direction_count = 1};
    Node south = {.position = (Point2D){.x = current.position.x, .y = current.position.y + 1},
                  .direction = SOUTH,
                  .direction_count = 1};
    Node east = {.position = (Point2D){.x = current.position.x + 1, .y = current.position.y},
                 .direction = EAST,
                 .direction_count = 1};
    Node west = {.position = (Point2D){.x = current.position.x - 1, .y = current.position.y},
                 .direction = WEST,
                 .direction_count = 1};
    switch (current.direction) {
    case EAST:
        add_node(rows, cols, north, neighbor, &count);
        add_node(rows, cols, south, neighbor, &count);
        if (!must_turn) {
            east.direction_count = current.direction_count + 1;
            add_node(rows, cols, east, neighbor, &count);
        }
        break;
    case WEST:
        add_node(rows, cols, north, neighbor, &count);
        add_node(rows, cols, south, neighbor, &count);
        if (!must_turn) {
            west.direction_count = current.direction_count + 1;
            add_node(rows, cols, west, neighbor, &count);
        }
        break;
    case SOUTH:
        add_node(rows, cols, west, neighbor, &count);
        add_node(rows, cols, east, neighbor, &count);
        if (!must_turn) {
            south.direction_count = current.direction_count + 1;
            add_node(rows, cols, south, neighbor, &count);
        }
        break;
    case NORTH:
        add_node(rows, cols, west, neighbor, &count);
        add_node(rows, cols, east, neighbor, &count);
        if (!must_turn) {
            north.direction_count = current.direction_count + 1;
            add_node(rows, cols, north, neighbor, &count);
        }
        break;
    }
    return count;
}

void solve(char *buf, size_t buf_size, Solution *result) {
    int part1 = 0, part2 = 0;

    int grid[GRID_SIZE][GRID_SIZE];
    int rows = 0, cols = 0;

    for (int pos = 0, x = 0; pos < (int)buf_size; pos++) {
        char c = buf[pos];
        switch (c) {
        case '\n':
            cols = x;
            x = 0;
            rows++;
            break;
        default: grid[rows][x++] = c - '0'; break;
        }
    }
    log_debug("rows: %d, cols: %d", rows, cols);

    Point2D start = {.x = 0, .y = 0}, dest = {.x = cols - 1, .y = rows - 1};
    Node start_node_1 = {.position = start, .direction = EAST, .direction_count = 0};
    Node start_node_2 = {.position = start, .direction = SOUTH, .direction_count = 0};

    // Dijkstra
    _cleanup_(pqu_State_free) pqu_State queue = pqu_State_init(State_compare);
    pqu_State_push(&queue, (State){.node = start_node_1, .dist = 0});
    pqu_State_push(&queue, (State){.node = start_node_2, .dist = 0});

    _cleanup_(ust_NodeDistance_free) ust_NodeDistance distances =
        ust_NodeDistance_init(NodeDistance_hash, NodeDistance_equal);
    ust_NodeDistance_insert(&distances, (NodeDistance){.node = start_node_1, .distance = 0});
    ust_NodeDistance_insert(&distances, (NodeDistance){.node = start_node_2, .distance = 0});

    // dijkstra
    while (!pqu_State_empty(&queue)) {
        State current = *pqu_State_top(&queue);
        pqu_State_pop(&queue);

        if (Point2D_equal(&current.node.position, &dest)) {
            log_debug("found destination %d,%d: %d", current.node.position.y, current.node.position.x, current.dist);
            part1 = current.dist;
            break;
        }

        Node neighbor[3];
        int neighbor_count = find_neighbors(rows, cols, current.node, neighbor);

        for (int i = 0; i < neighbor_count; i++) {
            Node nb = neighbor[i];
            int alt = current.dist + grid[nb.position.y][nb.position.x];
            NodeDistance node_distance = {.node = nb, .distance = alt};
            ust_NodeDistance_node *entry = ust_NodeDistance_find(&distances, node_distance);
            if (entry != NULL) {
                if (entry->key.distance <= alt) {
                    log_debug("discarding y,x %d,%d: current best %d is better than %d", nb.position.y, nb.position.x,
                              entry->key.distance, alt);
                    continue;
                }
            } else {
                log_debug("entry %d,%d not found in map", node_distance.node.position.y, node_distance.node.position.x);
            }
            log_debug("inserting new best dist for y,x %d,%d: %d", nb.position.y, nb.position.x, alt);
            ust_NodeDistance_insert(&distances, node_distance);
            pqu_State_push(&queue, (State){.node = nb, .dist = alt});
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
