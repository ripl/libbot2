// -*- mode: c -*-
// vim: set filetype=c :

/*
 * This file is part of bot2-lcm-utils.
 *
 * bot2-lcm-utils is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-lcm-utils is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-lcm-utils. If not, see <https://www.gnu.org/licenses/>.
 */

// signal_pipe was copied from bot_core to make lcm_tunnel a standalone pod

#ifndef BOT2_LCM_UTILS_TUNNEL_SIGNAL_PIPE_H_
#define BOT2_LCM_UTILS_TUNNEL_SIGNAL_PIPE_H_

/**
 * @defgroup BotCoreSignalPipe Signals to pipes
 * @brief Receive UNIX signal notifications via pipes
 * @ingroup BotCoreIO
 * @include: bot_core/bot_core.h
 *
 * signal_pipe provides convenience wrappers to convert unix signals into glib
 * events.
 *
 * e.g. to catch SIGINT in a gtk callback function, you might do:
 *
 * <programlisting>
 * void handle_sigint (int signal, void *user) {
 *     printf("caught SIGINT\n");
 *     gtk_main_quit();
 * }
 *
 * int main(int argc, char **argv) {
 *     gtk_init();
 *     bot_signal_pipe_init();
 *     bot_signal_pipe_add_signal (SIGINT);
 *     bot_signal_pipe_attach_glib (handle_sigint, NULL);
 *
 *     gtk_main();
 *
 *     bot_signal_pipe_destroy();
 * }
 * </programlisting>
 *
 * @{
 */

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*bot_signal_pipe_glib_handler_t)(int signal, void* user_data);

// initializes signal_pipe.  call this once per process.
int bot_signal_pipe_init(void);

// cleans up resources used by the signal_pipe
int bot_signal_pipe_cleanup(void);

// specifies that signal should be caught by signal_pipe and converted to a
// glib event
void bot_signal_pipe_add_signal(int signal);

// sets a handler function that is called when a signal is caught by
// signal_pipe.  The first argument to the user_func function is the number of
// the signal caught.  The second is the user_data parameter passed in here.
int bot_signal_pipe_attach_glib(bot_signal_pipe_glib_handler_t user_func,
                                gpointer user_data);

// convenience function to setup a signal handler that calls
// signal_pipe_init, and adds a signal handler that automatically call
// g_main_loop_quit (mainloop) on receiving SIGTERM, SIGINT, or SIGHUP.
// also invokes signal_pipe_cleanup() on receiving these signals.
int bot_signal_pipe_glib_quit_on_kill(GMainLoop* mainloop);

#ifdef __cplusplus
}  // extern "C"
#endif

/**
 * @}
 */

#endif  // BOT2_LCM_UTILS_TUNNEL_SIGNAL_PIPE_H_
