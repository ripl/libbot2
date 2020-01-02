/*
 * This file is part of bot2-lcmgl.
 *
 * bot2-lcmgl is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-lcmgl is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-lcmgl. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_LCMGL_BOT_LCMGL_RENDER_BOT_LCMGL_RENDERER_H_
#define BOT2_LCMGL_BOT_LCMGL_RENDER_BOT_LCMGL_RENDERER_H_

/**
 * @defgroup BotLCMGLViewerRenderer BotViewer renderer
 * @ingroup BotLCMGL
 * @brief BotVis Viewer renderer plugin
 * @include bot_lcmgl_render/lcmgl_bot_renderer.h
 *
 * TODO
 *
 * Linking: `pkg-config --libs bot2-lcmgl-renderer`
 * @{
 */

#include <lcm/lcm.h>

#include <bot_vis/viewer.h>

#ifdef __cplusplus
extern "C" {
#endif

void bot_lcmgl_add_renderer_to_viewer(BotViewer* viewer, lcm_t* lcm, int priority);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* BOT2_LCMGL_BOT_LCMGL_RENDER_BOT_LCMGL_RENDERER_H_ */
