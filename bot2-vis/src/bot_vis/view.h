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

#ifndef BOT2_VIS_BOT_VIS_VIEW_H_
#define BOT2_VIS_BOT_VIS_VIEW_H_

/**
 * @defgroup BotGlView BotGlView
 * @brief Viewpoints and OpenGL matrices
 * @ingroup BotVisGl
 * @include: bot_vis/bot_vis.h
 *
 * Linking: `pkg-config --libs bot2-vis`
 * @{
 */

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BotGlView BotGlView;

BotGlView * bot_gl_view_new(void);

void bot_gl_view_unref(BotGlView *view);

void bot_gl_view_setup_camera(BotGlView *self);

void bot_gl_view_look_at(BotGlView *self,
        const double eye[3], const double lookAt[3], const double up[3]);

void bot_gl_view_follow(BotGlView *self,
        double lastPos[3], double lastQuat[4],
        double newPos[3], double newQuat[4],
        gboolean followYaw);

// 1.0 for full perspective, 0.0 for orthographic. Interpolated otherwise.
void bot_gl_view_set_perspectiveness(BotGlView *self, double perspectiveness);

double bot_gl_view_get_perspectiveness(const BotGlView * self);

/**
 * bot_gl_view_get_projection_matrix:
 *
 * Retrieves the projection matrix as set by the view in column-major order
 */
void bot_gl_view_get_projection_matrix(const BotGlView *self, double m[16]);

/**
 * bot_gl_view_get_projection_matrix:
 *
 * Retrieves the modelview matrix as set by the view in column-major order
 */
void bot_gl_view_get_modelview_matrix(const BotGlView *self, double m[16]);

/**
 * bot_gl_view_get_viewport:
 *
 * Retrieves the last used viewport
 */
void bot_gl_view_get_viewport(const BotGlView *self, int viewport[4]);

void bot_gl_view_get_look_at(const BotGlView *self, double eye[3],
        double lookAt[3], double up[3]);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  /* BOT2_VIS_BOT_VIS_VIEW_H_ */
