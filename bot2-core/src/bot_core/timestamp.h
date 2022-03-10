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

#ifndef BOT2_CORE_BOT_CORE_TIMESTAMP_H_
#define BOT2_CORE_BOT_CORE_TIMESTAMP_H_

#include <stdint.h>
#include <sys/time.h>
// IWYU pragma: no_forward_declare timeval
#include <time.h>
// IWYU pragma: no_forward_declare timespec

/**
 * @defgroup BotCoreTimestamp Timestamp
 * @brief Retrieving the current time, synchronizing clocks
 * @ingroup BotCoreTime
 * @include: bot_core/bot_core.h
 *
 * Linking: `pkg-config --libs bot2-core`
 *
 * @{
 */

typedef struct bot_timestamp_sync_state {
  double dev_ticks_per_second;  // how fast does device clock count? (nominal)
  int64_t dev_ticks_wraparound;  // device clock counts modulo what?
  double max_rate_error;  // how fast do we need to count to ensure we're
                          // counting faster than device?

  int64_t
      sync_host_time;  // when we last synced, what time was it for the host?
  int64_t dev_ticks_since_sync;  // how many device ticks have elapsed since the
                                 // last sync?

  int64_t last_dev_ticks;  // what device time was it when we were last called?

  uint8_t is_valid;  // have we ever synced?
} bot_timestamp_sync_state_t;

#ifdef __cplusplus
extern "C" {
#endif

int64_t bot_timestamp_now(void);
int64_t bot_timestamp_seconds(int64_t v);
int64_t bot_timestamp_useconds(int64_t v);
void bot_timestamp_to_timeval(int64_t v, struct timeval* tv);
void bot_timestamp_to_timespec(int64_t v, struct timespec* ts);

/**
 * Create a new time synchronizer.
 * @param dev_ticks_per_second  The nominal rate at which the device time
 *                              increments
 * @param dev_ticks_wraparound  Assume that dev_ticks wraps around every
 *                              wraparound ticks
 * @param rate                  An upper bound on the rate error
 *
 * The syncronization algorithm is described in:
 * @inproceedings{olson2010,
 *     TITLE      = {A Passive Solution to the Sensor Synchronization Problem},
 *     AUTHOR     = {Edwin Olson},
 *     BOOKTITLE  = {Proceedings of the {IEEE/RSJ} International Conference on
 *                   Intelligent Robots and Systems {(IROS)}},
 *     YEAR       = {2010},
 *     MONTH      = {October},
 *     KEYWORDS   = {sensor calibration, time synchronization},
 * }
 */
bot_timestamp_sync_state_t* bot_timestamp_sync_init(
    double dev_ticks_per_second, int64_t dev_ticks_wraparound, double rate);
void bot_timestamp_sync_free(bot_timestamp_sync_state_t* s);
int64_t bot_timestamp_sync(bot_timestamp_sync_state_t* s, int64_t dev_ticks,
                           int64_t host_utime);

#ifdef __cplusplus
}  // extern "C"
#endif

/**
 * @}
 */

#endif  // BOT2_CORE_BOT_CORE_TIMESTAMP_H_
