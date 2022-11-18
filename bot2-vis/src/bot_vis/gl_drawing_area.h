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

#ifndef BOT2_VIS_BOT_VIS_GL_DRAWING_AREA_H_
#define BOT2_VIS_BOT_VIS_GL_DRAWING_AREA_H_

#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup BotGtkGlDrawingArea BotGTKGlDrawingArea
 * @brief GTK+ drawing-area widget with an OpenGL context
 * @ingroup BotVisGtk
 * @include: bot_vis/bot_vis.h
 *
 * Linking: `pkg-config --libs bot-vis`
 * @{
 */

#define BOT_GTK_TYPE_GL_DRAWING_AREA (bot_gtk_gl_drawing_area_get_type())
#define BOT_GTK_GL_DRAWING_AREA(obj)                               \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), BOT_GTK_TYPE_GL_DRAWING_AREA, \
                              BotGtkGlDrawingArea))
#define BOT_GTK_GL_DRAWING_AREA_CLASS(klass)                      \
  (G_TYPE_CHECK_CLASS_CAST((klass), BOT_GTK_TYPE_GL_DRAWING_AREA, \
                           BotGtkGlDrawingAreaClass))
#define BOT_GTK_IS_GL_DRAWING_AREA(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), BOT_GTK_TYPE_GL_DRAWING_AREA))
#define BOT_GTK_IS_GL_DRAWING_AREA_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), BOT_GTK_TYPE_GL_DRAWING_AREA))
#define BOT_GTK_GL_DRAWING_AREA_GET_CLASS(obj)                    \
  (G_TYPE_INSTANCE_GET_CLASS((obj), BOT_GTK_TYPE_GL_DRAWING_AREA, \
                             BotGtkGlDrawingAreaClass))

typedef struct _BotGtkGlDrawingArea {
  GtkDrawingArea area;

  gboolean vblank_sync;
} BotGtkGlDrawingArea;

typedef struct _BotGtkGlDrawingAreaClass {
  GtkDrawingAreaClass parent_class;
} BotGtkGlDrawingAreaClass;

GType bot_gtk_gl_drawing_area_get_type(void);
GtkWidget* bot_gtk_gl_drawing_area_new(gboolean vblank_sync);
void bot_gtk_gl_drawing_area_set_vblank_sync(BotGtkGlDrawingArea* glarea,
                                             gboolean vblank_sync);
void bot_gtk_gl_drawing_area_swap_buffers(BotGtkGlDrawingArea* glarea);
int bot_gtk_gl_drawing_area_set_context(BotGtkGlDrawingArea* glarea);
void bot_gtk_gl_drawing_area_invalidate(BotGtkGlDrawingArea* glarea);

/**
 * @}
 */

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_VIS_BOT_VIS_GL_DRAWING_AREA_H_
