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

#ifndef BOT2_FRAMES_BOT_FRAMES_BOT_FRAMES_RENDERERS_H_
#define BOT2_FRAMES_BOT_FRAMES_BOT_FRAMES_RENDERERS_H_

/*
 * coord_frames_renderer.h
 *
 *  Created on: Jan 22, 2011
 *      Author: abachrac
 */

#include <bot_param/param_client.h>
#include <bot_vis/viewer.h>

#include "bot_frames/bot_frames.h"

#ifdef __cplusplus
extern "C" {
#endif

//allow multiple simultaneous frames renders
void bot_frames_add_named_renderer_to_viewer(BotViewer *viewer, int render_priority, BotFrames * frames, const char * name);

void bot_frames_add_renderer_to_viewer(BotViewer *viewer, int render_priority, BotFrames * frames);

void bot_frames_add_articulated_body_renderer_to_viewer(BotViewer *viewer, int render_priority, BotParam * param,
    BotFrames * frames, const char * model_path, const char * param_articulated_name);

void bot_frames_add_frame_modifier_to_viewer(BotViewer *viewer, int render_priority, BotFrames * frames);

#ifdef __cplusplus
}
#endif

#endif  /* BOT2_FRAMES_BOT_FRAMES_BOT_FRAMES_RENDERERS_H_ */
