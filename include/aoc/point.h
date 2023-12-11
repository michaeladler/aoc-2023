/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stddef.h>

#include "aoc/macros.h"
#include "aoc/types.h"

#ifndef T
#define T i32
#endif

typedef struct {
    T x, y;
} Point2D;

static _unused_ int Point2D_equal(Point2D *lhs, Point2D *rhs) { return lhs->x == rhs->x && lhs->y == rhs->y; }

static _unused_ size_t Point2D_hash(Point2D *p) {
    const size_t prime = 31;
    size_t hash = 17;
    // Combine the hash of each struct member
    hash = hash * prime + (size_t)p->x;
    hash = hash * prime + (size_t)p->y;
    return hash;
}

static _unused_ T Point2D_manhattan(Point2D a, Point2D b) {
    T delta_x = a.x < b.x ? b.x - a.x : a.x - b.x;
    T delta_y = a.y < b.y ? b.y - a.y : a.y - b.y;
    return delta_x + delta_y;
}

#undef T
