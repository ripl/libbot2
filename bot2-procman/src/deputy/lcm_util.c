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

#include "lcm_util.h"

#include <stdio.h>
#include <stdlib.h>

#include <glib.h>

//#define dbg(...) fprintf (stderr, __VA_ARGS__)
#define dbg(...)

static int
lcm_message_ready (GIOChannel *source, GIOCondition cond, lcm_t *lcm)
{
    lcm_handle (lcm);
    return TRUE;
}

typedef struct {
    GIOChannel *ioc;
    guint sid;
    lcm_t *lcm;
} glib_attached_lcm_t;

static GHashTable *lcm_glib_sources = NULL;

int
lcmu_glib_mainloop_attach_lcm (lcm_t *lcm)
{
    if (!lcm_glib_sources) {
        lcm_glib_sources = g_hash_table_new (g_direct_hash, g_direct_equal);
    }

    if (g_hash_table_lookup (lcm_glib_sources, lcm)) {
        dbg ("lcm %p already attached to mainloop\n", lcm);
        return -1;
    }

    glib_attached_lcm_t *galcm =
        (glib_attached_lcm_t*) calloc (1, sizeof (glib_attached_lcm_t));

    galcm->ioc = g_io_channel_unix_new (lcm_get_fileno (lcm));
    galcm->sid = g_io_add_watch (galcm->ioc, G_IO_IN, (GIOFunc) lcm_message_ready,
            lcm);
    galcm->lcm = lcm;

    dbg ("inserted lcm %p into glib mainloop\n", lcm);
    g_hash_table_insert (lcm_glib_sources, lcm, galcm);
    return 0;
}

int
lcmu_glib_mainloop_detach_lcm (lcm_t *lcm)
{
    if (!lcm_glib_sources) {
        dbg ("no lcm glib sources\n");
        return -1;
    }

    glib_attached_lcm_t *galcm =
        (glib_attached_lcm_t*) g_hash_table_lookup (lcm_glib_sources, lcm);

    if (!galcm) {
        dbg ("couldn't find matching galcm\n");
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
    return 0;
}
