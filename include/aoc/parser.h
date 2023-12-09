/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "aoc/types.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * Parses a non-negative integer from a given string.
 *
 * This function attempts to parse and return a non-negative integer from the
 * provided character array. It begins parsing from the position pointed to by
 * `pos` and updates this value to the position immediately after the last
 * parsed character. This function skips any leading whitespace.
 *
 * If the parsing is successful, the function returns the parsed non-negative
 * integer. In case of an error, the function returns a negative number
 * indicating an error.
 *
 * @param buf A pointer to the null-terminated character array from which the
 * integer is parsed.
 * @param pos A pointer to a `size_t` variable. On function entry, it should
 * point to the index in `buf` from where the parsing should start. On function
 * exit, it is updated to the index immediately after the last parsed character.
 *
 * @return The parsed non-negative integer on success, or a negative number on
 * error.
 */
i64 aoc_parse_nonnegative(const char *buf, size_t *pos);

/**
 * Attempts to parse an integer from a given character buffer and stores the
 * result in an output parameter.
 *
 * This function scans the buffer pointed to by `buf` for an integer value,
 * starting from the current position indicated by `pos`. It interprets and
 * converts a sequence of characters representing a numerical value into an
 * integer (of type i64), and stores the result in the variable pointed to by
 * `out`. The position index pointed to by `pos` is updated to reflect the new
 * position in the buffer after parsing.
 *
 * @param buf A pointer to the character buffer from which the integer is to be
 * parsed.
 * @param pos A pointer to a size_t variable indicating the current position in
 * the buffer. After the function call, it will be updated to the position
 * following the parsed integer.
 * @param out A pointer to an i64 variable where the parsed integer value will
 * be stored.
 *
 * @return A boolean value. Returns true if an integer is successfully parsed
 * and stored in `out`, false otherwise. If false is returned, the contents of
 * `out` and the value at `pos` are indeterminate.
 */
bool aoc_parse_integer(const char *buf, size_t *pos, i64 *out);

/**
 * Skips over any whitespace characters in the buffer starting from the position
 * pointed by pos.
 *
 * @param buf The buffer to parse for whitespace characters.
 * @param pos Pointer to the size_t variable that holds the starting position
 * for skipping whitespace and is updated to the position after the skipped
 * whitespace.
 *
 * @note The function updates *pos to the position immediately following the
 * last whitespace character encountered. If there are no whitespace characters,
 * *pos remains unchanged.
 */
void aoc_parse_skip_ws(const char *buf, size_t *pos);

/**
 * Searches for the first occurrence of the specified character (needle) in the
 * buffer starting from the position pointed by pos.
 *
 * @param buf The buffer in which to search for the character.
 * @param pos Pointer to the size_t variable that holds the starting position
 * for the search and is updated to the position of the found character.
 * @param needle The character to search for in the buffer.
 *
 * @note The function updates *pos to the position of the first occurrence of
 * needle. If the needle is not found, *pos is set to the length of the buffer.
 */
void aoc_parse_seek(const char *buf, size_t *pos, char needle);
