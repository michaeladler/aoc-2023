/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>
#include <stdint.h>

#include "aoc/macros.h"
#include "aoc/math.h"
#include "aoc/string.h"

int aoc_gcdx(int a, int b, int *s, int *t) {
    int s0 = 1, s1 = 0, t0 = 0, t1 = 1;
    int q, r, m, n;

    while (a) {
        q = b / a;
        r = b % a;

        m = s0 - q * s1;
        n = t0 - q * t1;

        // Update b, a, s0, s1, t0, t1
        b = a;
        a = r;
        s0 = s1;
        s1 = m;
        t0 = t1;
        t1 = n;
    }

    *s = s0;
    *t = t0;

    return b;
}

int aoc_fastmod(int n, int mod) {
    n -= mod & -(n >= mod);
    return n;
}

int aoc_modinv(int b, int mod) {
    int x, y;
    aoc_gcdx(b, mod, &x, &y);
    x += mod & -(x < 0);
    return x;
}

// implements signum (-1, 0, or 1).
int aoc_sgn(int val) { return (0 < val) - (val < 0); }
