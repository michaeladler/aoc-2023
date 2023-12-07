/*
 * Author: Michael Adler
 *
 * Copyright: 2023 Michael Adler
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "hand.h"

hand_strength Hand_compute_strength_part2(Hand h);

void Hand_part2_tim_sort(Hand *dst, const size_t size);
