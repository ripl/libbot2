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

#include <stdio.h>
#include <stdlib.h>

#include <glib.h>
#include <lcm/lcm.h>

#include "bot_param/param_client.h"
#include "lcmtypes/bot_param_entry_t.h"
#include "lcmtypes/bot_param_set_t.h"
#include "../param_client/misc_utils.h"
#include "../param_client/param_internal.h"

int main(int argc, char ** argv)
{
  lcm_t * lcm = lcm_create(NULL); //TODO: provider options?

  if (argc != 3) {
    fprintf(stderr, "usage:\n %s <key> <param>\n", argv[0]);
    exit(1);
  }

  BotParam * param = bot_param_new_from_server(lcm, 0);
  bot_param_set_t msg;
  msg.utime = _timestamp_now();
  msg.sequence_number = bot_param_get_seqno(param);
  msg.server_id = bot_param_get_server_id(param);
  bot_param_entry_t entry;
  entry.key = argv[1];
  entry.value = argv[2];
  msg.numEntries = 1;
  msg.entries = &entry;

  char *param_prefix = getenv ("BOT_PARAM_SERVER_NAME");
  gchar *set_channel = g_strconcat (param_prefix ? : "",
          BOT_PARAM_SET_CHANNEL, NULL);
  bot_param_set_t_publish(lcm, set_channel, &msg);
  g_free (param_prefix);

  return 0;
}
