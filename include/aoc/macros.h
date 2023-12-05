/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stddef.h>

#define DIE(...)                                                               \
    do {                                                                       \
        fprintf(stderr, __VA_ARGS__);                                          \
        return -1;                                                             \
    } while (0)

#define _unused_ __attribute__((__unused__))
#define _cleanup_(x) __attribute__((__cleanup__(x)))
#define _likely_(x) (__builtin_expect(!!(x), 1))
#define _unlikely_(x) (__builtin_expect(!!(x), 0))
#define MAX(a, b)                                                              \
    ({                                                                         \
        typeof(a) _a = (a);                                                    \
        typeof(b) _b = (b);                                                    \
        _a > _b ? _a : _b;                                                     \
    })
#define MIN(a, b)                                                              \
    ({                                                                         \
        typeof(a) _a = (a);                                                    \
        typeof(b) _b = (b);                                                    \
        _a < _b ? _a : _b;                                                     \
    })

#define SWAP_VARS(x, y)                                                        \
    do {                                                                       \
        typeof(x) temp = (x);                                                  \
        (x) = (y);                                                             \
        (y) = temp;                                                            \
    } while (0)
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))
