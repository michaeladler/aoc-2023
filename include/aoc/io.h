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
 * Reads data from a file into a buffer.
 *
 * This function opens the file specified by `fname` and reads its contents
 * into a buffer `buf` provided by the caller. The amount of data read is
 * limited by `buf_size`.
 *
 * @param fname Pointer to a null-terminated string that specifies the name of
 * the file to be read.
 * @param buf Pointer to a buffer where the read data should be stored.
 * @param buf_size The size of the buffer `buf`, representing the maximum number
 * of bytes to be read.
 *
 * @return The function returns the number of bytes successfully read into the
 * buffer. If an error occurs, a negative value is returned to indicate the
 * error.
 */
int aoc_io_read_input(const char *fname, char *buf, size_t buf_size);
