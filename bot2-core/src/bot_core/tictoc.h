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

#ifndef BOT2_CORE_BOT_CORE_TICTOC_H_
#define BOT2_CORE_BOT_CORE_TICTOC_H_

// tictoc.h
//
//  Created on: May 29, 2009
//      Author: abachrac

#include <stdint.h>

/**
 * @defgroup BotCoreTicToc TicToc
 * @brief quick and dirty profiling tool.
 * @ingroup BotCoreTime
 * @include: bot_core/bot_core.h
 *
 * inspired by the matlab tic/toc command
 *
 * call bot_tictoc("description") to set the timer going
 * call it again with the same description to stop the timer
 *
 * Note: To get output, set the "BOT_TICTOC" environment variable to something
 *
 * @{
 */

#define BOT_TICTOC_ENV "BOT_TICTOC"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * bot_tictoc:
 *
 * basic invocation, the second time its called, it returns the time difference
 *in microseconds
 */
int64_t bot_tictoc(const char* description);

/**
 * bot_tictoc_full:
 *
 * full invocation, allows you to specify an
 * exponential moving average rate, and the current EMA value is returned in the
 * ema argument
 */
int64_t bot_tictoc_full(const char* description, double ema_alpha,
                        int64_t* ema);

/**
 * bot_tictoc_sort_type_t:
 *
 * Different Options for sorting the printed results
 */
typedef enum {
  BOT_TICTOC_AVG,
  BOT_TICTOC_TOTAL,
  BOT_TICTOC_MIN,
  BOT_TICTOC_MAX,
  BOT_TICTOC_EMA,
  BOT_TICTOC_ALPHABETICAL
} bot_tictoc_sort_type_t;

/**
 * bot_tictoc_print_stats:
 *
 * Print Out the stats from tictoc
 */
void bot_tictoc_print_stats(bot_tictoc_sort_type_t sortType);

/**
 * @}
 */

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_CORE_BOT_CORE_TICTOC_H_
