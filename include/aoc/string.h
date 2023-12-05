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

void aoc_strreverse(char *begin, char *end);

/**
 *  "itoa" based on Kernighan & Ritchie's "Ansi C".
 *  Converts value to a string and stores the result in str.
 *  Returns the length of str.
 */
size_t aoc_itoa(i64 value, char *out, int base);
