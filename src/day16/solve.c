/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "solve.h"
#include "aoc/all.h"

#define MAX_GRID_SIZE 110
#define MAX_BEAMS 4096

typedef enum { NORTH, EAST, SOUTH, WEST } direction_t;

typedef struct {
    int x;
    int y;
    direction_t direction;
} beam_t;

static inline void advance_beam(beam_t *beam) {
    switch (beam->direction) {
    case NORTH: beam->y--; break;
    case EAST: beam->x++; break;
    case SOUTH: beam->y++; break;
    case WEST: beam->x--; break;
    }
}

static inline char pretty_direction(direction_t d) {
    switch (d) {
    case NORTH: return 'N';
    case EAST: return 'E';
    case SOUTH: return 'S';
    case WEST: return 'W';
    default: return '?';
    }
}

void solve(char *buf, size_t buf_size, Solution *result) {
    char grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
    int rows = 0, cols = 0;

    size_t pos = 0;
    int x = 0;
    while (pos < buf_size) {
        char c = buf[pos++];
        if (c == '\n') {
            cols = x;
            x = 0;
            rows++;
            continue;
        }
        grid[rows][x++] = c;
    }
    log_debug("rows: %d, cols: %d", rows, cols);

    // each entry is a bitset containing the directions of the beam which visited it
    int visited[MAX_GRID_SIZE][MAX_GRID_SIZE];
    memset(visited, 0, sizeof(visited));

    beam_t beam[MAX_BEAMS];
    beam[0] = (beam_t){.x = 0, .y = 0, .direction = EAST};
    int beam_count = 1;
    while (beam_count > 0) {
        beam_t *b = &beam[beam_count - 1];
        if (b->x < 0 || b->x >= cols || b->y < 0 || b->y >= rows) {
            log_debug("beam leaves the grid, bye bye");
            beam_count--;
            continue;
        }

        // check if we've been here before
        int bitset = visited[b->y][b->x];
        int mask = 1 << b->direction;
        if (bitset & mask) {
            log_debug("!!! been here before, bye bye beam");
            beam_count--;
            continue;
        }
        log_debug("beam with direction %c visits %d, col %d (beam_count: %d)", pretty_direction(b->direction), b->y,
                  b->x, beam_count);
        visited[b->y][b->x] |= mask;

        switch (grid[b->y][b->x]) {
        case '/':
            switch (b->direction) {
            case NORTH: b->direction = EAST; break;
            case EAST: b->direction = NORTH; break;
            case SOUTH: b->direction = WEST; break;
            case WEST: b->direction = SOUTH; break;
            }
            log_debug("beam reflects and has new direction: %c", pretty_direction(b->direction));
            break;
        case '\\':
            switch (b->direction) {
            case NORTH: b->direction = WEST; break;
            case EAST: b->direction = SOUTH; break;
            case SOUTH: b->direction = EAST; break;
            case WEST: b->direction = NORTH; break;
            }
            log_debug("beam reflects and has new direction: %c", pretty_direction(b->direction));
            break;
        case '|':
            if (b->direction == EAST || b->direction == WEST) {
                log_debug("beam splits NORTH and SOUTH");
                b->direction = NORTH;
                beam[beam_count++] = (beam_t){.direction = SOUTH, .x = b->x, .y = b->y};
            }
            break;
        case '-':
            if (b->direction == NORTH || b->direction == SOUTH) {
                log_debug("beam splits EAST and WEST");
                b->direction = EAST;
                beam[beam_count++] = (beam_t){.direction = WEST, .x = b->x, .y = b->y};
            }
            break;
        }
        advance_beam(b);
    }

    int part1 = 0, part2 = 0;
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) { part1 += visited[y][x] != 0; }
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
