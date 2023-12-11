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
    Point2D p;
    int dist;
} QueueEntry;

#define P
#define T QueueEntry
#include <deq.h>

typedef struct {
    Point2D key;
    Point2D value;
} Entry;

static int Entry_equal(Entry *lhs, Entry *rhs) { return Point2D_equal(&lhs->key, &rhs->key); }

static size_t Entry_hash(Entry *e) { return Point2D_hash(&e->key); }

#define P
#define T Entry
#include <ust.h>

#define MAX_GRID_SIZE 140

typedef struct {
    char cells[MAX_GRID_SIZE][MAX_GRID_SIZE]; // rows (y), cols (x)
    int rows;
    int cols;
} Grid;

#define MOVE_NORTH(p, out, count)                                                                                      \
    do {                                                                                                               \
        if ((p).y - 1 >= 0) {                                                                                          \
            (out)[(count)].x = (p).x;                                                                                  \
            (out)[(count)].y = (p).y - 1;                                                                              \
            (count)++;                                                                                                 \
        }                                                                                                              \
    } while (0)
#define MOVE_SOUTH(p, out, count)                                                                                      \
    do {                                                                                                               \
        if ((p).y + 1 < grid.rows) {                                                                                   \
            (out)[(count)].x = (p).x;                                                                                  \
            (out)[(count)].y = (p).y + 1;                                                                              \
            (count)++;                                                                                                 \
        }                                                                                                              \
    } while (0)
#define MOVE_WEST(p, out, count)                                                                                       \
    do {                                                                                                               \
        if ((p).x - 1 >= 0) {                                                                                          \
            (out)[(count)].x = (p).x - 1;                                                                              \
            (out)[(count)].y = (p).y;                                                                                  \
            (count)++;                                                                                                 \
        }                                                                                                              \
    } while (0)
#define MOVE_EAST(p, out, count)                                                                                       \
    do {                                                                                                               \
        if ((p).x + 1 < grid.cols) {                                                                                   \
            (out)[(count)].x = (p).x + 1;                                                                              \
            (out)[(count)].y = (p).y;                                                                                  \
            (count)++;                                                                                                 \
        }                                                                                                              \
    } while (0)

static inline int find_neighbors(Grid grid, Point2D p, Point2D out[2]) {
    int count = 0;
    switch (grid.cells[p.y][p.x]) {
    case '|':
        MOVE_NORTH(p, out, count);
        MOVE_SOUTH(p, out, count);
        break;
    case '-':
        MOVE_WEST(p, out, count);
        MOVE_EAST(p, out, count);
        break;
    case 'L':
        MOVE_NORTH(p, out, count);
        MOVE_EAST(p, out, count);
        break;
    case 'J':
        MOVE_NORTH(p, out, count);
        MOVE_WEST(p, out, count);
        break;
    case '7':
        MOVE_SOUTH(p, out, count);
        MOVE_WEST(p, out, count);
        break;
    case 'F':
        MOVE_SOUTH(p, out, count);
        MOVE_EAST(p, out, count);
        break;
    }
    return count;
}

void solve(const char *buf, size_t buf_size, Solution *result) {
    int part1 = 0, part2 = 0;
    size_t pos = 0;

    Grid grid;
    Point2D s = {.x = -1, .y = -1};

    int x = 0, y = 0;
    { // parser
        while (buf[pos] != '\n') { grid.cells[y][x++] = buf[pos++]; }
        grid.cols = x;
        pos++, y++, x = 0; // newline

        while (pos < buf_size) {
            char c = buf[pos++];
            if (c == '\n') {
                x = 0, y++;
                continue;
            } else if (c == 'S') {
                s.x = x;
                s.y = y;
            }
            grid.cells[y][x] = c;
            x++;
        }
    }

    grid.rows = y;
    log_debug("cols: %d, rows: %d, S=(%d, %d)", grid.cols, grid.rows, s.x, s.y);

    const char candidates[] = {
        '|', '-', 'L', 'J', '7', 'F',
    };
    char start_char = '\0';

    Point2D loop[1 << 14];
    int loop_count = 0;

    for (size_t start_idx = 0; start_idx < ARRAY_LENGTH(candidates); start_idx++) {
        grid.cells[s.y][s.x] = candidates[start_idx];

        // dfs
        i8 seen[MAX_GRID_SIZE][MAX_GRID_SIZE] = {0};
        seen[s.y][s.x]++;

        _cleanup_(deq_QueueEntry_free) deq_QueueEntry queue = deq_QueueEntry_init();
        {
            QueueEntry entry = {.p = s, .dist = 0};
            deq_QueueEntry_push_back(&queue, entry);
        }

        _cleanup_(ust_Entry_free) ust_Entry parent_map = ust_Entry_init(Entry_hash, Entry_equal);

        Point2D neighbor[2];
        while (!deq_QueueEntry_empty(&queue)) {
            QueueEntry current = *deq_QueueEntry_back(&queue);
            deq_QueueEntry_pop_back(&queue);
            for (int i = 0; i < find_neighbors(grid, current.p, neighbor); i++) {
                Point2D p = neighbor[i];
                if (seen[p.y][p.x] == 0) {
                    seen[p.y][p.x]++;

                    // parent of p is current
                    Entry entry = {.key = p, .value = current.p};
                    ust_Entry_insert(&parent_map, entry);

                    QueueEntry qentry = {.p = p, .dist = current.dist + 1};
                    deq_QueueEntry_push_back(&queue, qentry);
                } else {
                    // if a node has already been visited and is not the
                    // parent of the current node, a cycle is detected.
                    ust_Entry_node *node = ust_Entry_find(&parent_map, (Entry){.key = p});
                    if (node && !Point2D_equal(&node->key.value, &current.p)) {
                        Point2D parent = node->key.value;
                        if (Point2D_equal(&s, &parent)) {
                            int cycle_len = 1 + (current.dist / 2);
                            log_debug("found cycle to start: %d", cycle_len);
                            if (cycle_len > 2) {
                                start_char = candidates[start_idx];

                                Point2D tmp = current.p;
                                while ((node = ust_Entry_find(&parent_map, (Entry){.key = tmp})) != NULL) {
                                    loop[loop_count++] = node->key.key;
                                    tmp = node->key.value;
                                }

                                tmp = p;
                                while ((node = ust_Entry_find(&parent_map, (Entry){.key = tmp})) != NULL) {
                                    loop[loop_count++] = node->key.key;
                                    tmp = node->key.value;
                                }

                                goto dfs_done;
                            }
                        }
                    }
                }
            }
        }
    }
dfs_done:

    part1 = (loop_count + 1) / 2;

    // part 2
    Grid new_grid;
    new_grid.rows = grid.rows;
    new_grid.cols = grid.cols;
    memset(new_grid.cells, '.', grid.rows * grid.cols);
    for (int i = 0; i < loop_count; i++) {
        Point2D p = loop[i];
        new_grid.cells[p.y][p.x] = grid.cells[p.y][p.x];
    }
    new_grid.cells[s.y][s.x] = start_char;

    // use the Even-odd algorithm, see https://en.wikipedia.org/wiki/Even%E2%80%93odd_rule
    char corner[1024];
    for (int y = 0; y < new_grid.rows; y++) {
        for (int x = 0; x < new_grid.cols; x++) {
            char c = new_grid.cells[y][x];
            if (c != '.') continue;

            int intersection_count = 0;
            int corner_count = 0;
            for (int i = x + 1; i < new_grid.cols; i++) { // walk east
                char c_east = new_grid.cells[y][i];
                if (c_east == '|') {
                    intersection_count++;
                } else if (c_east == 'F' || c_east == 'L') {
                    corner[corner_count++] = c_east;
                } else if (corner_count > 0 && ((c_east == 'J' && corner[corner_count - 1] == 'F') ||
                                                (c_east == '7' && corner[corner_count - 1] == 'L'))) {
                    corner_count--;
                    intersection_count++;
                }
            }

            if (intersection_count % 2 == 1) {
                part2 += 1; // inner point
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
