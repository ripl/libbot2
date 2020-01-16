// -*- mode: c -*-
// vim: set filetype=c :

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

#ifndef BOT2_LCMGL_BOT_LCMGL_RENDER_LCMGL_DECODE_H_
#define BOT2_LCMGL_BOT_LCMGL_RENDER_LCMGL_DECODE_H_

#include <stdint.h>

/**
 * @defgroup BotLCMGLRender LCMGL decoding and rendering
 * @ingroup BotLCMGL
 * @brief Executing OpenGL commands received via LCMGL
 * @include bot_lcmgl_render/lcmgl_decode.h
 *
 * TODO
 *
 * Linking: `pkg-config --libs bot2-lcmgl-renderer`
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * bot_lcmgl_decode:
 *
 * Decodes a block of LCMGL data, and executes the OpenGL commands with
 * the current OpenGL context.
 */
void bot_lcmgl_decode(uint8_t* data, int datalen);

/**
 * @}
 */

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_LCMGL_BOT_LCMGL_RENDER_LCMGL_DECODE_H_
