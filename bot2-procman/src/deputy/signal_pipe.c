// -*- mode: c -*-
// vim: set filetype=c :

/*
 * This file is part of bot2-procman.
 *
 * bot2-procman is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-procman is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-procman. If not, see <https://www.gnu.org/licenses/>.
 */

#include "signal_pipe.h"

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define dbg(args...) fprintf(stderr, args)
#undef dbg
#define dbg(args...)

typedef struct _signal_pipe {
  int fds[2];
  GIOChannel* ioc;
  guint ios;

  signal_pipe_glib_handler_t userfunc;
  void* userdata;
} signal_pipe_t;

static signal_pipe_t g_sp;
static int g_sp_initialized = 0;

int signal_pipe_init() {
  if (g_sp_initialized) {
    fprintf(stderr, "signal_pipe already initialized!!\n");
    return -1;
  }

  if (0 != pipe(g_sp.fds)) {
    perror("signal_pipe");
    return -1;
  }

  int flags = fcntl(g_sp.fds[1], F_GETFL);
  fcntl(g_sp.fds[1], F_SETFL, flags | O_NONBLOCK);

  g_sp_initialized = 1;

  dbg("signal_pipe: initialized\n");
  return 0;
}

int signal_pipe_cleanup() {
  if (g_sp_initialized) {
    close(g_sp.fds[0]);
    close(g_sp.fds[1]);
    g_io_channel_unref(g_sp.ioc);
    g_sp_initialized = 0;
    return 0;
  }

  dbg("signal_pipe: destroyed\n");
  return -1;
}

static void signal_handler(int signal) {
  dbg("signal_pipe: caught signal %d\n", signal);
  int wstatus = write(g_sp.fds[1], &signal, sizeof(int));
  (void)wstatus;
}

static int signal_handler_glib(GIOChannel* source, GIOCondition condition,
                               void* ud) {
  int signal;
  int status;
  status = read(g_sp.fds[0], &signal, sizeof(int));

  if (status != sizeof(int)) {
    fprintf(stderr, "wtf!? signal_handler_glib is confused (%s:%d)\n", __FILE__,
            __LINE__);
    return TRUE;
  }

  if (g_sp.userfunc) {
    g_sp.userfunc(signal, g_sp.userdata);
  }

  return TRUE;
}

void signal_pipe_add_signal(int sig) {
  // TODO(ashuang): use sigaction instead of signal()
  signal(sig, signal_handler);
}

int signal_pipe_attach_glib(signal_pipe_glib_handler_t func,
                            gpointer user_data) {
  if (!g_sp_initialized) {
    return -1;
  }

  if (g_sp.ioc) {
    return -1;
  }

  g_sp.ioc = g_io_channel_unix_new(g_sp.fds[0]);
  g_io_channel_set_flags(
      g_sp.ioc, g_io_channel_get_flags(g_sp.ioc) | G_IO_FLAG_NONBLOCK, NULL);
  g_sp.ios = g_io_add_watch(g_sp.ioc, G_IO_IN | G_IO_PRI,
                            (GIOFunc)signal_handler_glib, NULL);

  g_sp.userfunc = func;
  g_sp.userdata = user_data;

  return 0;
}

static void spgqok_handler(int signal, void* user) {
  GMainLoop* mainloop = (GMainLoop*)user;
  g_main_loop_quit(mainloop);
  signal_pipe_cleanup();
}

int signal_pipe_glib_quit_on_kill(GMainLoop* mainloop) {
  if (0 != signal_pipe_init()) {
    return -1;
  }

  signal_pipe_add_signal(SIGINT);
  signal_pipe_add_signal(SIGTERM);
  signal_pipe_add_signal(SIGKILL);
  signal_pipe_add_signal(SIGHUP);

  return signal_pipe_attach_glib(spgqok_handler, mainloop);
}
