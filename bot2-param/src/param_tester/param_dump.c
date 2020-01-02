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

#include <stdio.h>
#include <stdlib.h>

#include <lcm/lcm.h>

#include "lcmtypes/bot_param_update_t.h"
#include "../param_client/param_internal.h"

static void _on_param_update(const lcm_recv_buf_t *rbuf, const char * channel, const bot_param_update_t * msg,
    void * user)
{
  printf("%s", msg->params);
  exit(1);
}

int main()
{
  lcm_t * lcm = lcm_create(NULL);

  bot_param_update_t_subscribe(lcm, BOT_PARAM_UPDATE_CHANNEL, _on_param_update, NULL);

  while (1)
    lcm_handle(lcm);
}
