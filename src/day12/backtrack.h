#pragma once

#include "spring.h"

struct BT {
    spring_t spring;
    int idx_unknown[MAX_UNKNOWNS];
    int idx_unknown_count;
    int group[MAX_GROUPS]; // e.g. 1,1,3 means groups of damaged are 1, 1 and 3, i.e. 5 total damaged
    int group_count;

    int solution_count;
};

typedef struct BT *data; /* type to pass data to backtrack */

void backtrack(int a[], int k, data input);
