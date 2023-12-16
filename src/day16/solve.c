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

static char grid[MAX_GRID_SIZE][MAX_GRID_SIZE];

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

static int run_simulation(beam_t start, int rows, int cols) {
    // each entry is a bitset containing the directions of the beam which visited it
    int visited[MAX_GRID_SIZE][MAX_GRID_SIZE];
    memset(visited, 0, sizeof(visited));

    beam_t beam[MAX_BEAMS];
    beam[0] = start;
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

    int sum = 0;
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) { sum += visited[y][x] != 0; }
    }
    return sum;
}

void solve(char *buf, size_t buf_size, Solution *result) {
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

    // brute force
    int part1 = 0, part2 = 0;
    for (direction_t direction = 0; direction < 4; direction++) {
        // top row
        if (direction != NORTH) {
            for (int x = 0; x < cols; x++) {
                beam_t start = (beam_t){.x = x, .y = 0, .direction = direction};
                int value = run_simulation(start, rows, cols);
                if (direction == EAST && x == 0) part1 = value;
                part2 = MAX(part2, value);
            }
        }
        // bottom row
        if (direction != SOUTH) {
            for (int x = 0; x < cols; x++) {
                beam_t start = (beam_t){.x = x, .y = rows - 1, .direction = direction};
                int value = run_simulation(start, rows, cols);
                part2 = MAX(part2, value);
            }
        }
        // left row
        if (direction != WEST) {
            for (int y = 0; y < rows; y++) {
                beam_t start = (beam_t){.x = 0, .y = y, .direction = direction};
                int value = run_simulation(start, rows, cols);
                part2 = MAX(part2, value);
            }
        }
        // right row
        if (direction != EAST) {
            for (int y = 0; y < rows; y++) {
                beam_t start = (beam_t){.x = cols - 1, .y = y, .direction = direction};
                int value = run_simulation(start, rows, cols);
                part2 = MAX(part2, value);
            }
        }
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
