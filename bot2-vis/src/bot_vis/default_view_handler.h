/*
 * This file is part of bot2-vis.
 *
 * bot2-vis is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-vis is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-vis. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_VIS_BOT_VIS_DEFAULT_VIEW_HANDLER_H_
#define BOT2_VIS_BOT_VIS_DEFAULT_VIEW_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "viewer.h"

typedef struct _BotDefaultViewHandler
{
    BotEventHandler ehandler;
    BotViewHandler  vhandler;
    BotViewer* viewer;

    //goal view
    double goal_eye[3];
    double goal_lookat[3];
    double goal_up[3];

    //initial view
    double origin_eye[3];
    double origin_lookat[3];
    double origin_up[3];

    double viewpath_duration_ms;
    int64_t viewpath_timer_start;

    int width, height;
    double aspect_ratio;

    double fov_degrees;

    double last_mouse_x;
    double last_mouse_y;

    int projection_type;

    // these three variables determine the position and orientation of
    // the camera.
    double lookat[3];
    double eye[3];
    double up[3];

    // when there's a mouse press in the scene, we determine which
    // point in the scene is the one being manipulated. This
    // manipulation point is preserved under the mouse cursor during
    // any pan operations.
    double manipulation_point[4];

    // should be recomputed whenever look/eye/up is modified.  must be
    // kept in-sync with look/eye/up
    double model_matrix[16];

    // should be recomputed whenever camera is modified.
    double projection_matrix[16];

    int viewport[4];

    // these are used to implement follow: they correspond to the last
    // position of the object.  we manipulate the lookat/eye/up values
    // to make the position of the object invariant to motions of this
    // point.
    int have_last;
    double lastpos[3], lastquat[4];
} BotDefaultViewHandler;

BotDefaultViewHandler *bot_default_view_handler_new(BotViewer *viewer);

#ifdef __cplusplus
}
#endif

#endif  /* BOT2_VIS_BOT_VIS_DEFAULT_VIEW_HANDLER_H_ */
