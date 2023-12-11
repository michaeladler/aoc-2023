/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stddef.h>

typedef struct {
    int x;
    int y;
} Point2D;

inline int Point2D_equal(Point2D *lhs, Point2D *rhs) { return lhs->x == rhs->x && lhs->y == rhs->y; }

inline size_t Point2D_hash(Point2D *p) {
    const size_t prime = 31;
    size_t hash = 17;
    // Combine the hash of each struct member
    hash = hash * prime + (unsigned int)p->x;
    hash = hash * prime + (unsigned int)p->y;
    return hash;
}
