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

#ifndef BOT2_VIS_BOT_VIS_FBGL_DRAWING_AREA_H_
#define BOT2_VIS_BOT_VIS_FBGL_DRAWING_AREA_H_

#include <glib.h>
#include <glib-object.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/**
 * @defgroup BotFBGLDrawingArea OpenGL offscreen rendering
 * @brief Offscreen rendering using Framebuffer objects
 * @ingroup BotVisGl
 * @include: bot_vis/bot_vis.h
 *
 * TODO
 *
 * Linking: `pkg-config --libs bot2-vis`
 * @{
 */

G_BEGIN_DECLS

#define BOT_TYPE_FBGL_DRAWING_AREA            (bot_fbgl_drawing_area_get_type ())
#define BOT_FBGL_DRAWING_AREA(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BOT_TYPE_FBGL_DRAWING_AREA, BotFbglDrawingArea))
#define BOT_FBGL_DRAWING_AREA_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), BOT_TYPE_FBGL_DRAWING_AREA, BotFbglDrawingAreaClass))
#define FB_IS_GL_DRAWING_AREA(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BOT_TYPE_FBGL_DRAWING_AREA))
#define FB_IS_GL_DRAWING_AREA_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BOT_TYPE_FBGL_DRAWING_AREA))
#define BOT_FBGL_DRAWING_AREA_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), BOT_TYPE_FBGL_DRAWING_AREA, BotFbglDrawingAreaClass))

typedef struct _BotFbglDrawingArea {
    GObject parent;

    int width, height;
} BotFbglDrawingArea;

typedef struct _BotFbglDrawingAreaClass {
    GObjectClass parent;
} BotFbglDrawingAreaClass;

GType       bot_fbgl_drawing_area_get_type (void);
BotFbglDrawingArea * bot_fbgl_drawing_area_new (gboolean new_context,
        int width, int height, GLenum format);
void        bot_fbgl_drawing_area_swap_buffers (BotFbglDrawingArea * glarea);
int         bot_fbgl_drawing_area_begin (BotFbglDrawingArea * glarea);
int         bot_fbgl_drawing_area_end (BotFbglDrawingArea * glarea);
int         bot_fbgl_drawing_area_flush (BotFbglDrawingArea * glarea);

G_END_DECLS

/**
 * @}
 */

#endif  /* BOT2_VIS_BOT_VIS_FBGL_DRAWING_AREA_H_ */
