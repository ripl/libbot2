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

#include <stdlib.h>

#include <glib.h>
#include <gtk/gtk.h>
#include <lcm/lcm.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <bot_core/lcm_util.h>
#include <bot_lcmgl_render/lcmgl_bot_renderer.h>
#include <bot_param/param_client.h>
#include <bot_vis/gtk_util.h>
#include <bot_vis/viewer.h>

#include "bot_frames_renderers.h"
#include "bot_frames/bot_frames.h"

static void on_top_view_clicked(GtkToggleToolButton *tb, void *user_data)
{
  BotViewer *self = (BotViewer*) user_data;

  double eye[3];
  double look[3];
  double up[3];
  self->view_handler->get_eye_look(self->view_handler, eye, look, up);

  eye[0] = 0;
  eye[1] = 0;
  eye[2] = 10;
  look[0] = 0;
  look[1] = 0;
  look[2] = 0;
  up[0] = 0;
  up[1] = 10;
  up[2] = 0;
  self->view_handler->set_look_at(self->view_handler, eye, look, up);

  bot_viewer_request_redraw(self);
}

int main(int argc, char *argv[])
{
  gtk_init(&argc, &argv);
  glutInit(&argc, argv);

  //  if (argc < 2) {
  //    fprintf(stderr, "usage: %s <render_plugins>\n", g_path_get_basename(argv[0]));
  //    exit(1);
  //  }
  lcm_t * lcm = bot_lcm_get_global(NULL);
  BotParam * param = bot_param_get_global(lcm, 0);
  BotFrames * bcf = bot_frames_get_global(lcm, param);
  bot_glib_mainloop_attach_lcm(lcm);

  BotViewer* viewer = bot_viewer_new("Bot Frames Test Viewer");
  //die cleanly for control-c etc :-)
  bot_gtk_quit_on_interrupt();

  // setup renderers
  bot_viewer_add_stock_renderer(viewer, BOT_VIEWER_STOCK_RENDERER_GRID, 1);
  bot_lcmgl_add_renderer_to_viewer(viewer, lcm, 1);
  bot_frames_add_renderer_to_viewer(viewer, 1, bcf);
  bot_frames_add_articulated_body_renderer_to_viewer(viewer, 1, param, bcf, NULL, "articulated_body_name");

  //load the renderer params from the config file.
  char *fname = g_build_filename(g_get_user_config_dir(), ".bft-viewerrc", NULL);
  bot_viewer_load_preferences(viewer, fname);

  gtk_main();

  //save the renderer params to the config file.
  bot_viewer_save_preferences(viewer, fname);

  bot_viewer_unref(viewer);
}
