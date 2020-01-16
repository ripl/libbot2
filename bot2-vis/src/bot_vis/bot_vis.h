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

#ifndef BOT2_VIS_BOT_VIS_BOT_VIS_H_
#define BOT2_VIS_BOT_VIS_BOT_VIS_H_

// IWYU pragma: begin_exports
#include "batch_gl.h"
#include "console.h"
#include "default_view_handler.h"
#include "fbgl_drawing_area.h"
#include "gl_drawing_area.h"
#include "gl_image_area.h"
#include "gl_util.h"
#include "gtk_util.h"
#include "param_widget.h"
#include "rwx.h"
#include "scrollplot2d.h"
#include "texture.h"
#include "view.h"
#include "viewer.h"
#include "wavefront.h"
// IWYU pragma: end_exports

/**
 * @defgroup BotVis Bot Vis
 * @brief Visualization with OpenGL and GTK+
 */

/**
 * @defgroup BotVisGtk GTK+
 * @brief GTK+ Widgets utility functions
 * @ingroup BotVis
 */

/**
 * @defgroup BotVisGl OpenGL
 * @brief OpenGL data structures and utility functions
 * @ingroup BotVis
 */

/**
 * @defgroup BotViewerGroup Viewer
 * @brief Visualization with OpenGL and GTK+
 * @ingroup BotVis
 */

#endif  // BOT2_VIS_BOT_VIS_BOT_VIS_H_
