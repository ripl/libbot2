// -*- mode: c -*-
// vim: set filetype=c :

/*
 * This file is part of bot2-core.
 *
 * bot2-core is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-core is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-core. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_CORE_BOT_CORE_RAND_UTIL_H_
#define BOT2_CORE_BOT_CORE_RAND_UTIL_H_

/**
 * @defgroup BotCoreRandUtil Rand Utilities
 * @ingroup BotCoreMathGeom
 * @brief Miscellaneous random number utility functions
 * @include: bot_core/bot_core.h
 *
 * Linking: `pkg-config --libs bot2-core`
 *
 * @{
 */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// random number between [0, 1)
static inline float bot_randf() { return ((float)rand()) / (RAND_MAX + 1.0); }

// random number between (-1, 1)
static inline float bot_signed_randf() { return bot_randf() * 2.0 - 1.0; }
// random number between [mi, ma ]
static inline float bot_randf_in_range(float mi, float ma) {
  return bot_randf() * (ma - mi) + mi;
}

// return a random integer between [0, bound)
static inline int bot_irand(int bound) {
  int v = (int)(bot_randf() * bound);
  assert(v >= 0);
  assert(v < bound);
  return v;
}

// seed bot_gauss_rand (defaults to 13 if bot_gauss_rand() called before
// bot_gauss_rand_init()
void bot_gauss_rand_init(uint32_t seed);

// return a normally distributed random number
double bot_gauss_rand(double mu, double sigma);

/**
 * @}
 */

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_CORE_BOT_CORE_RAND_UTIL_H_
