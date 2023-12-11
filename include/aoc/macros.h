/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stddef.h>

#define DIE(...)                                                                                                       \
    do {                                                                                                               \
        fprintf(stderr, __VA_ARGS__);                                                                                  \
        return -1;                                                                                                     \
    } while (0)

#define _unused_ __attribute__((__unused__))
#define _cleanup_(x) __attribute__((__cleanup__(x)))
#define _likely_(x) (__builtin_expect(!!(x), 1))
#define _unlikely_(x) (__builtin_expect(!!(x), 0))
#define MAX(a, b)                                                                                                      \
    ({                                                                                                                 \
        typeof(a) _a = (a);                                                                                            \
        typeof(b) _b = (b);                                                                                            \
        _a > _b ? _a : _b;                                                                                             \
    })
#define MIN(a, b)                                                                                                      \
    ({                                                                                                                 \
        typeof(a) _a = (a);                                                                                            \
        typeof(b) _b = (b);                                                                                            \
        _a < _b ? _a : _b;                                                                                             \
    })

#define SWAP_VARS(x, y)                                                                                                \
    do {                                                                                                               \
        typeof(x) temp = (x);                                                                                          \
        (x) = (y);                                                                                                     \
        (y) = temp;                                                                                                    \
    } while (0)
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

/**
 * Accesses an element in a one-dimensional array as if it were a two-dimensional array.
 *
 * This macro calculates the index in a one-dimensional array corresponding to
 * the (x, y) position in a two-dimensional array layout. The two-dimensional
 * array is assumed to be row-major, where each row contains 'W' elements.
 *
 * @param arr The one-dimensional array.
 * @param x The x-coordinate (column index) in the two-dimensional layout.
 * @param y The y-coordinate (row index) in the two-dimensional layout.
 * @param W The width of the two-dimensional array, i.e., the number of elements in each row.
 * @return The reference to the array element at the specified (x, y) position.
 *
 * Example usage:
 *   int myArray[10]; // A one-dimensional array
 *   int W = 5;       // Width of the two-dimensional array representation
 *   INDEX(myArray, 2, 1, W) = 42; // Sets the element at (x=2, y=1) to 42
 */
#define INDEX(arr, x, y, W) ((arr)[(y) * (W) + (x)])
