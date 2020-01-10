/*
 * This file is part of bot2-param.
 *
 * bot2-param is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-param is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-param. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_PARAM_BOT_PARAM_MISC_UTILS_H_
#define BOT2_PARAM_BOT_PARAM_MISC_UTILS_H_

/*
 * misc_utils.h
 *
 *  Created on: Oct 2, 2010
 *      Author: abachrac
 */

#include <stdint.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
/* IWYU pragma: no_forward_declare timeval */

#include <lcm/lcm.h>

/**
 * lcm_sleep:
 * @lcm: The lcm_t object.
 * @sleeptime max time to wait in seconds
 *
 *  Waits for up to @sleeptime seconds for an LCM message to arrive.
 *  It handles the first message if one arrives.
 *
 */
static inline void lcm_sleep(lcm_t * lcm, double sleeptime)
{
  int lcm_fileno = lcm_get_fileno(lcm);

  fd_set rfds;
  int retval;
  FD_ZERO(&rfds);
  FD_SET(lcm_fileno, &rfds);
  struct timeval tv;
  tv.tv_sec = (int) sleeptime;
  tv.tv_usec = (int) ((sleeptime - tv.tv_sec) * 1.0e6);
  retval = select(lcm_fileno + 1, &rfds, NULL, NULL, &tv);
  if (retval == -1) {
    fprintf(stderr, "bot_lcm_poll: select() failed!\n");
    return;
  }
  if (retval) {
    if (FD_ISSET(lcm_fileno, &rfds)) {
      lcm_handle(lcm);
    }
  }
}

static inline int64_t _timestamp_now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int64_t) tv.tv_sec * 1000000 + tv.tv_usec;
}

#endif  /* BOT2_PARAM_BOT_PARAM_MISC_UTILS_H_ */
