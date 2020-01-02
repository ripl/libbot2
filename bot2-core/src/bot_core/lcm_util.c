/*
 * This file is part of bot2-core.
 *
 * bot2-core is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-core is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-core. If not, see <https://www.gnu.org/licenses/>.
 */

#include "lcm_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>

#include "timestamp.h"

//#define dbg(...) fprintf (stderr, __VA_ARGS__)
#define dbg(...)

typedef struct {
    GIOChannel *ioc;
    guint sid;
    lcm_t *lcm;
    gboolean quit_on_lcm_fail;
    GMainLoop * mainloop;
} glib_attached_lcm_t;

static int
lcm_message_ready (GIOChannel *source, GIOCondition cond, void *user_data)
{
    glib_attached_lcm_t *galcm = (glib_attached_lcm_t*) user_data;
    if (0 != lcm_handle (galcm->lcm) && galcm->quit_on_lcm_fail) {
        if(galcm->mainloop) {
            g_main_loop_quit(galcm->mainloop);
            return FALSE;
        }
    }
    return TRUE;
}

static GHashTable *lcm_glib_sources = NULL;
static GMutex lcm_glib_sources_mutex;
static lcm_t *global_lcm = NULL;

int
bot_glib_mainloop_attach_lcm (lcm_t *lcm)
{
    return bot_glib_mainloop_attach_lcm_full(NULL, lcm, FALSE);
}

int
bot_glib_mainloop_attach_lcm_full (GMainLoop * mainloop, lcm_t *lcm,
        gboolean quit_on_lcm_fail)
{
    g_mutex_lock (&lcm_glib_sources_mutex);

    if (!lcm_glib_sources) {
        lcm_glib_sources = g_hash_table_new (g_direct_hash, g_direct_equal);
    }

    if (g_hash_table_lookup (lcm_glib_sources, lcm)) {
        dbg ("lcm %p already attached to mainloop\n", lcm);
        g_mutex_unlock (&lcm_glib_sources_mutex);
        return -1;
    }

    glib_attached_lcm_t *galcm =
        (glib_attached_lcm_t*) calloc (1, sizeof (glib_attached_lcm_t));

    galcm->lcm = lcm;
    galcm->quit_on_lcm_fail = quit_on_lcm_fail;
    galcm->mainloop = mainloop;
    galcm->ioc = g_io_channel_unix_new (lcm_get_fileno (lcm));
    galcm->sid = g_io_add_watch (galcm->ioc, G_IO_IN,
            (GIOFunc) lcm_message_ready, galcm);

    dbg ("inserted lcm %p into glib mainloop\n", lcm);
    g_hash_table_insert (lcm_glib_sources, lcm, galcm);

    g_mutex_unlock (&lcm_glib_sources_mutex);
    return 0;
}

int
bot_glib_mainloop_detach_lcm (lcm_t *lcm)
{
    g_mutex_lock (&lcm_glib_sources_mutex);
    if (!lcm_glib_sources) {
        dbg ("no lcm glib sources\n");
        g_mutex_unlock (&lcm_glib_sources_mutex);
        return -1;
    }

    glib_attached_lcm_t *galcm =
        (glib_attached_lcm_t*) g_hash_table_lookup (lcm_glib_sources, lcm);

    if (!galcm) {
        dbg ("couldn't find matching galcm\n");
        g_mutex_unlock (&lcm_glib_sources_mutex);
        return -1;
    }

    dbg ("detaching lcm from glib\n");
    g_io_channel_unref (galcm->ioc);
    g_source_remove (galcm->sid);

    g_hash_table_remove (lcm_glib_sources, lcm);
    free (galcm);

    if (g_hash_table_size (lcm_glib_sources) == 0) {
        g_hash_table_destroy (lcm_glib_sources);
        lcm_glib_sources = NULL;
    }

    g_mutex_unlock (&lcm_glib_sources_mutex);
    return 0;
}

lcm_t *
bot_lcm_get_global(const char *provider)
{
    g_mutex_lock (&lcm_glib_sources_mutex);
    if(!global_lcm)
        global_lcm = lcm_create(provider);
    g_mutex_unlock (&lcm_glib_sources_mutex);
    return global_lcm;
}

void bot_lcm_handle_or_timeout(lcm_t * lcm, int64_t timeout)
{
  int lcm_fileno = lcm_get_fileno(lcm);

  fd_set rfds;
  int retval;
  FD_ZERO(&rfds);
  FD_SET(lcm_fileno, &rfds);
  struct timeval tv;
  bot_timestamp_to_timeval(timeout,&tv);
  retval = select(lcm_fileno + 1, &rfds, NULL, NULL, &tv);
  if (retval == -1) {
    fprintf(stderr, "bot_lcm_handle_or_timeout: select() failed!\n");
    return;
  }
  if (retval) {
    if (FD_ISSET(lcm_fileno, &rfds)) {
      lcm_handle(lcm);
    }
  }
}
