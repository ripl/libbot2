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

#ifndef BOT2_CORE_BOT_CORE_GLIB_UTIL_H_
#define BOT2_CORE_BOT_CORE_GLIB_UTIL_H_

/**
 * SECTION:glib_util
 * @title:GLib Utilities
 * @short_description: useful functions missing from GLib
 * @include: bot_core/bot_core.h
 *
 * Linking: `pkg-config --libs bot2-core`
 */

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

// calls func on every element of the specified list, and then
// frees the list
void bot_g_list_free_with_func (GList *list, GDestroyNotify func);

// calls func on every element of the specified queue, and then
// frees the queue
void bot_g_queue_free_with_func (GQueue *queue, GDestroyNotify func);

// calls func on every element of the specified pointer array, and then
// frees the pointer array.
void bot_g_ptr_array_free_with_func (GPtrArray *a, GDestroyNotify func);

// creates a newly allocated copy of a GPtrArray
GPtrArray * bot_g_ptr_array_new_copy (const GPtrArray *a);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
/**
 * Returns: 1 if time1 is after time2
 *          0 if time1 and time2 are equal
 *         -1 if time1 is before time2
 */
int bot_g_time_val_compare (const GTimeVal *time1, const GTimeVal *time2);
#pragma GCC diagnostic pop

GList * bot_g_hash_table_get_keys (GHashTable *hash_table);

GList * bot_g_hash_table_get_vals (GHashTable *hash_table);

GPtrArray * bot_g_hash_table_get_vals_array (GHashTable *hash_table);

#ifndef bot_g_ptr_array_size
#define bot_g_ptr_array_size(ptrarray) ((ptrarray)->len)
#endif

//
//#ifndef g_ptr_array_set
//#define g_ptr_array_set(ptrarray, idx, val) (ptrarray)->pdata[(idx)] = (val);
//#endif
//
int bot_g_ptr_array_find_index(GPtrArray *a, gconstpointer v);
//#define gu_ptr_array_find_index g_ptr_array_find_index
//
guint bot_pint64_hash(gconstpointer _key);
gboolean bot_pint64_equal(gconstpointer _a, gconstpointer _b);

#ifdef __cplusplus
}
#endif

#endif  /* BOT2_CORE_BOT_CORE_GLIB_UTIL_H_ */
