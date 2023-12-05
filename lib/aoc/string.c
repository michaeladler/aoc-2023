/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>

#include "aoc/string.h"

void aoc_strreverse(char *begin, char *end) {
    char aux;
    while (end > begin) aux = *end, *end-- = *begin, *begin++ = aux;
}

size_t aoc_itoa(int value, char *str, int base) {
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";

    char *wstr = str;
    int sign;

    // Validate base
    assert(base >= 2 && base <= 35);

    // Take care of sign
    if ((sign = value) < 0) value = -value;

    // Conversion. Number is reversed.
    do { *wstr++ = num[value % base]; } while (value /= base);

    if (sign < 0) *wstr++ = '-';

    *wstr = '\0';
    aoc_strreverse(str, wstr - 1);
    return wstr - str;
}
