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

#ifndef BOT2_VIS_BOT_VIS_GTK_UTIL_H_
#define BOT2_VIS_BOT_VIS_GTK_UTIL_H_

/**
 * @defgroup BotVisGtkUtil Miscellaneous GTK+ utility functions
 * @brief GTK utility functions
 * @ingroup BotVisGtk
 * @include: bot_vis/bot_vis.h
 *
 * Linking: `pkg-config --libs bot2-vis`
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Adds an event handler to the GTK mainloop that calls gtk_main_quit() when
 * SIGINT, SIGTERM, or SIGHUP are received
 */
int bot_gtk_quit_on_interrupt(void);

#ifdef __cplusplus
}  // extern "C"
#endif

/**
 * @}
 */

#endif  // BOT2_VIS_BOT_VIS_GTK_UTIL_H_
