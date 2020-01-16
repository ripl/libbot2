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

#ifndef BOT2_CORE_BOT_CORE_TIMESPEC_H_
#define BOT2_CORE_BOT_CORE_TIMESPEC_H_

#include <sys/time.h>
// IWYU pragma: no_forward_declare timeval
#include <time.h>
// IWYU pragma: no_forward_declare timespec

/**
 * @defgroup BotCoreTimeSpec Timespec
 * @brief Convenience functions for <literal>struct timespec</literal>
 * @ingroup BotCoreTime
 * @include: bot_core/bot_core.h
 *
 * TODO
 *
 * Linking: `pkg-config --libs bot2-core`
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

// get the current time
void bot_timespec_now(struct timespec* ts);

// add ms milliseconds to the timespec (ms > 0)
void bot_timespec_addms(struct timespec* ts, long ms);

// add ns nanoseconds to the timespec (ns > 0)
void bot_timespec_addns(struct timespec* ts, long ns);

void bot_timespec_adjust(struct timespec* ts, double dt);

// compare a and b
int bot_timespec_compare(struct timespec* a, struct timespec* b);

// display the timespec
void bot_timespec_print(struct timespec* a);

// computes a = a-b
void bot_timespec_subtract(struct timespec* a, struct timespec* b);

// convert the timespec into milliseconds (may overflow)
int bot_timespec_milliseconds(struct timespec* a);

void bot_timeval_set(struct timeval* tv, double dt);

void bot_timespec_to_timeval(struct timespec* ts, struct timeval* tv);

#ifdef __cplusplus
}  // extern "C"
#endif

/**
 * @}
 */

#endif  // BOT2_CORE_BOT_CORE_TIMESPEC_H_
