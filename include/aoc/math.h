/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stddef.h>

#include "aoc/types.h"

/**
 * Calculate the greatest common divisor (GCD) of two integers 'a' and 'b'
 * using the extended Euclidean algorithm. Additionally, compute the Bézout
 * coefficients 's' and 't' such that:
 *
 *   gcd(a, b) = a*s + b*t
 *
 * The function updates the values pointed to by 's' and 't' to store the
 * Bézout coefficients.
 *
 * @param a The first integer
 * @param b The second integer
 * @param s Pointer to an integer to store the Bézout coefficient 's'
 * @param t Pointer to an integer to store the Bézout coefficient 't'
 * @return The greatest common divisor (GCD) of 'a' and 'b'
 */
i64 aoc_gcdx(i64 a, i64 b, i64 *s, i64 *t);

/**
 * Calculate the lowest common multiple (LCM) of two integers 'a' and 'b'.
 */
i64 aoc_lcm(i64 a, i64 b);

/**
 * Calculate the lowest common multiple (LCM) of an array of integers.
 */
i64 aoc_lcm_many(i64 arr[], size_t n);

/**
 * Calculate the modular residue of an integer 'a' modulo 'm' using
 * a faster computation technique suitable for positive integers.
 *
 * @param a The integer to compute the modular residue for
 * @param m The modulus value
 * @return The modular residue of 'a' modulo 'm'
 */
i64 aoc_fastmod(i64 a, i64 m);

/**
 * Calculate the modular multiplicative inverse of an integer 'a' modulo 'm'.
 * The function finds an integer 'x' such that (a * x) % m == 1.
 *
 * @param a The integer for which to find the modular inverse
 * @param m The modulus value
 * @return The modular multiplicative inverse of 'a' modulo 'm', or -1 if no
 *         inverse exists (when 'a' and 'm' are not coprime).
 */
i64 aoc_modinv(i64 a, i64 m);

/**
 * Determine the sign of an integer 'val'.
 *
 * @param val The integer to determine the sign of
 * @return  1 if 'val' is positive,
 *         -1 if 'val' is negative,
 *          0 if 'val' is zero.
 */
i64 aoc_sgn(i64 val);
