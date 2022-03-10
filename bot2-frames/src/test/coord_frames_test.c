// -*- mode: c -*-
// vim: set filetype=c :

/*
 * This file is part of bot2-frames.
 *
 * bot2-frames is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-frames is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-frames. If not, see <https://www.gnu.org/licenses/>.
 */

// coord_frame_test.c
//
//  Created on: Jan 14, 2011
//      Author: abachrac

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <lcm/lcm.h>

#include <bot_core/rotations.h>
#include <bot_core/timestamp.h>
#include <bot_core/trans.h>
#include <bot_param/param_client.h>
#include <lcmtypes/bot_core_rigid_transform_t.h>

#include "bot_frames/bot_frames.h"

void update_handler(BotFrames* bot_frames, const char* frame,
                    const char* relative_to, int64_t utime, void* user) {
  printf("link %s->%s was updated, user = %p\n", frame, relative_to, user);
}

int main(int argc, char** argv) {
  lcm_t* lcm = lcm_create(NULL);
  BotParam* param = bot_param_new_from_server(lcm, 0);
  BotFrames* bcf = bot_frames_get_global(lcm, param);
  bot_frames_add_update_subscriber(bcf, update_handler, NULL);

  for (int i = 0; i < 100; i++) {
    BotTrans t;
    bot_frames_get_trans(bcf, "laser", "local", &t);
    fprintf(stderr, "laser->local= (%f,%f,%f) - (%f,%f,%f,%f)\n",
            t.trans_vec[0], t.trans_vec[1], t.trans_vec[2], t.rot_quat[0],
            t.rot_quat[1], t.rot_quat[2], t.rot_quat[3]);
    bot_core_rigid_transform_t msg;
    msg.utime = bot_timestamp_now();
    msg.trans[0] = ((float)rand()) / (RAND_MAX + 1.0);
    msg.trans[1] = ((float)rand()) / (RAND_MAX + 1.0);
    msg.trans[2] = ((float)rand()) / (RAND_MAX + 1.0);

    double rod[3];
    rod[0] = ((float)rand()) / (RAND_MAX + 1.0);
    rod[1] = ((float)rand()) / (RAND_MAX + 1.0);
    rod[2] = ((float)rand()) / (RAND_MAX + 1.0);
    bot_rodrigues_to_quat(rod, msg.quat);

    bot_core_rigid_transform_t_publish(lcm, "BODY_TO_LOCAL", &msg);
    lcm_handle(lcm);
  }

  return 0;
}
