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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <lcm/lcm.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <bot_core/lcm_util.h>
#include <bot_vis/viewer.h>

#include "bot_lcmgl_render/lcmgl_bot_renderer.h"
#include "udp_util.h"
#include "view_menu.h"

typedef struct {
  BotViewer* viewer;
  lcm_t* lcm;
} state_t;

static int logplayer_remote_on_key_press(BotViewer* viewer,
                                         BotEventHandler* ehandler,
                                         const GdkEventKey* event) {
  int keyval = event->keyval;

  switch (keyval) {
    case 'P':
    case 'p':
      udp_send_string("127.0.0.1", 53261, "PLAYPAUSETOGGLE");
      break;
    case 'N':
    case 'n':
      udp_send_string("127.0.0.1", 53261, "STEP");
      break;
    case '=':
    case '+':
      udp_send_string("127.0.0.1", 53261, "FASTER");
      break;
    case '_':
    case '-':
      udp_send_string("127.0.0.1", 53261, "SLOWER");
      break;
    case '[':
      udp_send_string("127.0.0.1", 53261, "BACK5");
      break;
    case ']':
      udp_send_string("127.0.0.1", 53261, "FORWARD5");
      break;
    default:
      return 0;
  }

  return 1;
}

/////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
  gtk_init(&argc, &argv);
  glutInit(&argc, argv);

  setlinebuf(stdout);

  state_t app;
  memset(&app, 0, sizeof(app));

  BotViewer* viewer = bot_viewer_new("Viewer");
  app.viewer = viewer;
  app.lcm = lcm_create(NULL);
  bot_glib_mainloop_attach_lcm(app.lcm);

  setup_view_menu(viewer);

  // setup renderers
  bot_viewer_add_stock_renderer(viewer, BOT_VIEWER_STOCK_RENDERER_GRID, 1);
  bot_lcmgl_add_renderer_to_viewer(viewer, app.lcm, 0);

  // logplayer controls
  BotEventHandler* ehandler =
      (BotEventHandler*)calloc(1, sizeof(BotEventHandler));
  ehandler->name = "LogPlayer Remote";
  ehandler->enabled = 1;
  ehandler->key_press = logplayer_remote_on_key_press;
  bot_viewer_add_event_handler(viewer, ehandler, 0);

  // load saved preferences
  char* fname =
      g_build_filename(g_get_user_config_dir(), ".lcmgl-viewerrc", NULL);
  bot_viewer_load_preferences(viewer, fname);

  // run the main loop
  gtk_main();

  // save any changed preferences
  bot_viewer_save_preferences(viewer, fname);
  free(fname);

  // cleanup
  bot_viewer_unref(viewer);
}
