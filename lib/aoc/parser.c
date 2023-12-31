/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "aoc/parser.h"

void aoc_parse_skip_ws(const char *buf, size_t *pos) {
    int i = *pos;
    while (buf[i] == ' ') i++;
    *pos = i;
    assert(buf[*pos] != ' ');
}

i64 aoc_parse_nonnegative(const char *buf, size_t *pos) {
    aoc_parse_skip_ws(buf, pos);

    i64 result = 0;
    size_t i = *pos;
    while (buf[i] >= '0' && buf[i] <= '9') {
        result = (result * 10) + (buf[i] - '0');
        i++;
    }
    if (i == *pos) { // error, nothing parsed
        return -1;
    }
    *pos = i;
    return result;
}

bool aoc_parse_integer(const char *buf, size_t *pos, i64 *out) {
    aoc_parse_skip_ws(buf, pos);
    if (buf[*pos] == '-') {
        *pos = *pos + 1;
        i64 value = aoc_parse_nonnegative(buf, pos);
        if (value < 0) { return false; }
        *out = 0 - value;
        return true;
    }
    if (buf[*pos] == '+') { *pos = *pos + 1; }
    i64 value = aoc_parse_nonnegative(buf, pos);
    if (value < 0) { return false; }
    *out = value;
    return true;
}

void aoc_parse_seek(const char *buf, size_t *pos, char needle) {
    int i = *pos;
    while (buf[i] != needle) i++;
    *pos = i;
    assert(buf[*pos] == needle);
}
