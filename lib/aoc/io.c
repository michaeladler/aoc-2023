/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include "aoc/io.h"
#include "aoc/macros.h"

/* IO */
static inline void my_fclose(FILE **fp) {
    FILE *f = *fp;
    if (f) fclose(f);
}

int aoc_io_read_input(const char *fname, char *buf, size_t buf_size) {
    _cleanup_(my_fclose) FILE *f = fopen(fname, "r");
    if (!f) {
        perror("Error opening input:");
        return -1;
    }

    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    if (fsize >= buf_size) {
        fprintf(stderr, "File (%zu) too large for buffer (%zu)\n", fsize,
                buf_size);
        return -2;
    }
    rewind(f);

    size_t end = fread(buf, sizeof(char), fsize, f);
    if (end != fsize) {
        perror("Error reading file");
        return -3;
    }
    /* ensure buf is null-terminated */
    buf[end] = '\0';
    return end;
}
