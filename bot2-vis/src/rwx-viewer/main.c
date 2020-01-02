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

#include <stdio.h>
#include <stdlib.h>

#include <glib.h>
#include <gtk/gtk.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "bot_vis/viewer.h"

void setup_renderer_rwx(BotViewer *viewer, int render_priority, const char *rwx_fname);

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    glutInit(&argc, argv);

    if(argc < 2) {
        fprintf(stderr, "usage: %s <rwx_filename>\n",
                g_path_get_basename(argv[0]));
        exit(1);
    }

    const char *rwx_fname = argv[1];

    BotViewer* viewer = bot_viewer_new("RWX Viewer");

    char *fname = g_build_filename(g_get_user_config_dir(), ".rwx-viewerrc", NULL);

    bot_viewer_load_preferences(viewer, fname);

    // setup renderers
    bot_viewer_add_stock_renderer(viewer, BOT_VIEWER_STOCK_RENDERER_GRID, 1);
    setup_renderer_rwx(viewer, 1, rwx_fname);

    gtk_main();

    bot_viewer_save_preferences(viewer, fname);

    bot_viewer_unref(viewer);
}
