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

i64 aoc_gcdx(i64 a, i64 b, i64 *s, i64 *t) {
    i64 s0 = 1, s1 = 0, t0 = 0, t1 = 1;
    i64 q, r, m, n;

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

    if (s != NULL) *s = s0;
    if (t != NULL) *t = t0;

    return b;
}

i64 aoc_lcm(i64 a, i64 b) {
    i64 gcd = aoc_gcdx(a, b, NULL, NULL);
    return (a / gcd) * b;
}

i64 aoc_lcm_many(i64 arr[], size_t n) {
    i64 ans = arr[0];
    for (size_t i = 1; i < n; i++) { ans = aoc_lcm(ans, arr[i]); }
    return ans;
}

i64 aoc_fastmod(i64 a, i64 m) {
    a -= m & -(a >= m);
    return a;
}

i64 aoc_modinv(i64 a, i64 m) {
    i64 x, y;
    aoc_gcdx(a, m, &x, &y);
    x += m & -(x < 0);
    return x;
}

i64 aoc_sgn(i64 val) { return (0 < val) - (val < 0); }
