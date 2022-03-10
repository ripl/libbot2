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

// clang-format off
#include "bot_vis/param_widget.h"  // IWYU pragma: associated
// clang-format on

#include <limits.h>
#include <stdio.h>
#include <string.h>

enum {
  MENU_1,
  MENU_2,
  MENU_3,
};

static void on_param_changed(BotGtkParamWidget* pw, const char* name,
                             void* user_data) {
  if (!strcmp(name, "Integer 1")) {
    printf("int1: %d\n", bot_gtk_param_widget_get_int(pw, name));
  } else if (!strcmp(name, "Integer Slider")) {
    printf("int slider: %d\n", bot_gtk_param_widget_get_int(pw, name));
  } else if (!strcmp(name, "Double Slider")) {
    printf("double slider: %f\n", bot_gtk_param_widget_get_double(pw, name));
  } else if (!strcmp(name, "check1") || !strcmp(name, "check2") ||
             !strcmp(name, "check3")) {
    printf("%s: %s\n", name,
           bot_gtk_param_widget_get_bool(pw, name) ? "True" : "False");
  } else if (!strcmp(name, "Menu")) {
    printf("menu: %s / %d\n", bot_gtk_param_widget_get_enum_str(pw, name),
           bot_gtk_param_widget_get_enum(pw, name));
  }
}

int main(int argc, char** argv) {
  printf("param_widget_test\n");

  GtkWidget* window;
  BotGtkParamWidget* pw;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(window), "delete_event", gtk_main_quit, NULL);
  g_signal_connect(G_OBJECT(window), "destroy", gtk_main_quit, NULL);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  pw = BOT_GTK_PARAM_WIDGET(bot_gtk_param_widget_new());
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(pw));

  bot_gtk_param_widget_add_int(pw, "Integer 1", BOT_GTK_PARAM_WIDGET_SPINBOX,
                               INT_MIN, INT_MAX, 1, 10);

  bot_gtk_param_widget_add_int(pw, "Integer Slider",
                               BOT_GTK_PARAM_WIDGET_SLIDER, 0, 100, 50, 1);

  bot_gtk_param_widget_add_double(pw, "Double Slider",
                                  BOT_GTK_PARAM_WIDGET_SLIDER, 0, 1, 0.01, 0.5);

  bot_gtk_param_widget_add_booleans(pw, BOT_GTK_PARAM_WIDGET_CHECKBOX, "check1",
                                    0, "check2", 1, "check3", 0, NULL);

  bot_gtk_param_widget_add_enum(pw, "Menu", BOT_GTK_PARAM_WIDGET_MENU, MENU_2,
                                "entry 1", MENU_1, "entry 2", MENU_2, "entry 3",
                                MENU_3, NULL);

  g_signal_connect(G_OBJECT(pw), "changed", G_CALLBACK(on_param_changed), NULL);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
