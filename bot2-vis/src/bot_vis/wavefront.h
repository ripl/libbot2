// -*- mode: c -*-
// vim: set filetype=c :

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

#ifndef BOT2_VIS_BOT_VIS_WAVEFRONT_H_
#define BOT2_VIS_BOT_VIS_WAVEFRONT_H_

// wavefront.h
//
// Public API for rendering 3D models that are represented in the
// Wavefront OBJ (geometry) and MTL (material properties) file formats.
//
// The API is largely a wrapper for the GLM library
//
// Created on: Dec 2, 2010
//     Author: mwalter

/**
 * @defgroup BotVisWavefront Wavefront (.obj:geometry; .mtl:material)
 *           mesh mesh model rendering
 * @brief Parsing and rendering .obj geometry models and their corresponding
          .mtl material properties definition
 * @ingroup BotVisGl
 * @include: bot_vis/bot_vis.h
 *
 * Linking: `pkg-config --libs bot2-vis`
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BotWavefrontModel BotWavefrontModel;

/**
 * bot_wavefront_model_create:
 * @filename: The name of the model's Wavefront .obj file
 *
 * Reads the model represented as a Wavefront .obj file.
 *
 * Returns: A pointer to a newly-allocated %BotWavefrontModel
 *          or %NULL on parse error.
 */
BotWavefrontModel* bot_wavefront_model_create(const char* filename);

/**
 * bot_wavefront_model_destroy:
 * @model: The %BotWavefrontModel to free
 *
 * Frees the provided %BotWavefrontModel
 */
void bot_wavefront_model_destroy(BotWavefrontModel* model);

/**
 * bot_wavefront_model_get_extrema:
 * @model: The %BotWavefrontModel
 *
 * Determins the min/max extrema over vertices
 */
void bot_wavefront_model_get_extrema(BotWavefrontModel* model, double minv[3],
                                     double maxv[3]);

/**
 * bot_wavefront_model_gl_draw:
 * @model: The %BotWavefrontModel
 *
 * Draws the model using opengl
 */
void bot_wavefront_model_gl_draw(BotWavefrontModel* model);

#ifdef __cplusplus
}  // extern "C"
#endif

/**
 * @}
 */

#endif  // BOT2_VIS_BOT_VIS_WAVEFRONT_H_
