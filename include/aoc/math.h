/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stddef.h>

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
int aoc_gcdx(int a, int b, int *x, int *y);

/**
 * Calculate the modular residue of an integer 'n' modulo 'mod' using
 * a faster computation technique suitable for positive integers.
 *
 * @param n The integer to compute the modular residue for
 * @param mod The modulus value
 * @return The modular residue of 'n' modulo 'mod'
 */
int aoc_fastmod(int n, int mod);

/**
 * Calculate the modular multiplicative inverse of an integer 'b' modulo 'mod'.
 * The function finds an integer 'x' such that (b * x) % mod == 1.
 *
 * @param b The integer for which to find the modular inverse
 * @param mod The modulus value
 * @return The modular multiplicative inverse of 'b' modulo 'mod', or -1 if no
 *         inverse exists (when 'b' and 'mod' are not coprime).
 */
int aoc_modinv(int b, int mod);

/**
 * Determine the sign of an integer 'val'.
 *
 * @param val The integer to determine the sign of
 * @return  1 if 'val' is positive,
 *         -1 if 'val' is negative,
 *          0 if 'val' is zero.
 */
int aoc_sgn(int val);
