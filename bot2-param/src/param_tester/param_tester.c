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

/*
 * conf_tester.cpp
 *
 *  Created on: Sep 13, 2010
 *      Author: abachrac
 */
// reading a complete binary file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <lcm/lcm.h>

#include "bot_param/param_client.h"

void param_update_handler(BotParam * old_param,BotParam * new_param, int64_t utime, void *user){
  fprintf(stderr, "some parameters were updated %p!\n",user);
}

int main()
{
  lcm_t * lcm = lcm_create(NULL);

  BotParam * param = bot_param_new_from_server(lcm, 1);
  if (param == NULL) {
    fprintf(stderr, "could not get params!\n");
    exit(1);
  }

  bot_param_add_update_subscriber(param,param_update_handler,lcm);

  char * s;
  int ret = bot_param_write_to_string(param, &s);
  fprintf(stderr, "%s", s);
  free(s);

  bot_param_write(param, stderr);

  while (1) {
    lcm_handle(lcm);
    char * key = "coordinate_frames.body.history";
    fprintf(stderr, "%s = %d\n", key, bot_param_get_int_or_fail(param, key));
  }
  return 0;
}
