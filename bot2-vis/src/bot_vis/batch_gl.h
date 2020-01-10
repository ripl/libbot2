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

#ifndef BOT2_VIS_BOT_VIS_BATCH_GL_H_
#define BOT2_VIS_BOT_VIS_BATCH_GL_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup BotVisBatchGL Batched OpenGL commands
 * @brief Batching OpenGL commands for delayed rendering
 * @ingroup BotVisGl
 * @include: bot_vis/bot_vis.h
 *
 * Batched GL drawing allows a program to issue some basic OpenGL drawing
 * commands while an OpenGL context is not active.  Commands will be queued up,
 * stored, and then executed when requested via bot_bgl_render().
 *
 * Linking: `pkg-config --libs bot2-vis`
 *
 * @{
 */

typedef struct _BotBatchGl BotBatchGl;

BotBatchGl *bot_bgl_new (void);
void bot_bgl_destroy (BotBatchGl *bgl);

void bot_bgl_begin (BotBatchGl *bgl, GLenum mode);
void bot_bgl_end (BotBatchGl *bgl);

void bot_bgl_vertex2d (BotBatchGl *bgl, double v0, double v1);
void bot_bgl_vertex2dv(BotBatchGl *bgl, const double *v);
void bot_bgl_vertex2f (BotBatchGl *bgl, float v0, float v1);
void bot_bgl_vertex2fv(BotBatchGl *bgl, const float *v);

void bot_bgl_vertex3d (BotBatchGl *bgl, double v0, double v1, double v2);
void bot_bgl_vertex3dv(BotBatchGl *bgl, const double *v);
void bot_bgl_vertex3f (BotBatchGl *bgl, float v0, float v1, float v2);
void bot_bgl_vertex3fv(BotBatchGl *bgl, const float *v);

void bot_bgl_color3f (BotBatchGl *bgl, float v0, float v1, float v);
void bot_bgl_color4f (BotBatchGl *bgl, float v0, float v1, float v2, float v3);
void bot_bgl_point_size (BotBatchGl *bgl, float v);
void bot_bgl_line_width (BotBatchGl *bgl, float line_width);

void bot_bgl_enable (BotBatchGl *bgl, GLenum v);
void bot_bgl_disable (BotBatchGl *bgl, GLenum v);

void bot_bgl_blend_func (BotBatchGl *bgl, GLenum sfactor, GLenum dfactor);

void BotBatchGlranslated (BotBatchGl *bgl, double v0, double v1, double v2);
void BotBatchGlranslatef (BotBatchGl *bgl, float v0, float v1, float v2);
void bot_bgl_rotated (BotBatchGl *bgl, double angle, double x, double y, double z);
void bot_bgl_rotatef (BotBatchGl *bgl, float angle, float x, float y, float z);
void bot_bgl_push_matrix (BotBatchGl * bgl);
void bot_bgl_pop_matrix (BotBatchGl * bgl);
void bot_bgl_load_identity (BotBatchGl *bgl);

void bot_bgl_mult_matrixd (BotBatchGl *bgl, const double *matrix);
void bot_bgl_mult_matrixf (BotBatchGl *bgl, const float *matrix);

void bot_bgl_render (BotBatchGl *bgl);
void bot_bgl_switch_buffer (BotBatchGl *bgl);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* BOT2_VIS_BOT_VIS_BATCH_GL_H_ */
