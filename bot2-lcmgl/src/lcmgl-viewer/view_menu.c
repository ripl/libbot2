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

#include "view_menu.h"

#include <glib-object.h>
#include <gtk/gtk.h>

static void on_setview_y_up_item(GtkMenuItem* mi, void* user) {
  BotViewer* viewer = (BotViewer*)user;
  double eye[] = {0, 0, 50};
  double lookat[] = {0, 0, 0};
  double up[] = {0, 1, 0};
  if (viewer->view_handler) {
    viewer->view_handler->set_look_at(viewer->view_handler, eye, lookat, up);
  }
}

static void on_setview_x_up_item(GtkMenuItem* mi, void* user) {
  BotViewer* viewer = (BotViewer*)user;
  double eye[] = {0, 0, 50};
  double lookat[] = {0, 0, 0};
  double up[] = {1, 0, 0};
  if (viewer->view_handler) {
    viewer->view_handler->set_look_at(viewer->view_handler, eye, lookat, up);
  }
}

void setup_view_menu(BotViewer* viewer) {
  GtkWidget* view_menu = viewer->view_menu;

  // bookmarks

  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu),
                        gtk_separator_menu_item_new());

  // predefined viewpoints
  GtkWidget* setview_y_up_item = gtk_menu_item_new_with_mnemonic("Y axis up");
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), setview_y_up_item);
  g_signal_connect(G_OBJECT(setview_y_up_item), "activate",
                   G_CALLBACK(on_setview_y_up_item), viewer);

  // predefined viewpoints
  GtkWidget* setview_x_up_item = gtk_menu_item_new_with_mnemonic("X axis up");
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), setview_x_up_item);
  g_signal_connect(G_OBJECT(setview_x_up_item), "activate",
                   G_CALLBACK(on_setview_x_up_item), viewer);

  gtk_widget_show_all(view_menu);
}
