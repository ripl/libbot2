// -*- mode: c -*-
// vim: set filetype=c :

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

#ifndef BOT2_CORE_BOT_CORE_SET_H_
#define BOT2_CORE_BOT_CORE_SET_H_

#include <glib.h>

/**
 * @defgroup BotCoreSet Set
 * @brief Set data structure for hashable objects
 * @ingroup BotCoreDataStructures
 * @include: bot_core/bot_core.h
 *
 * TODO
 *
 * Linking: `pkg-config --libs bot2-core`
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BotSet BotSet;

BotSet* bot_set_new(GHashFunc hash_func, GEqualFunc equal_func);

BotSet* bot_set_new_full(GHashFunc hash_func, GEqualFunc equal_func,
                         GDestroyNotify element_destroy_func);

BotSet* bot_set_new_union(const BotSet* set1, const BotSet* set2);

BotSet* bot_set_new_intersection(const BotSet* set1, const BotSet* set2);

BotSet* bot_set_new_copy(const BotSet* set);

/**
 * bot_set_subtract:
 * removes elements from set1 that are also in set2
 */
void bot_set_subtract(BotSet* set1, const BotSet* set2);

void bot_set_destroy(BotSet* set);

void bot_set_add(BotSet* set, gpointer element);

void bot_set_add_list(BotSet* set, GList* list);

void bot_set_remove(BotSet* set, gpointer element);

void bot_set_remove_all(BotSet* set);

int bot_set_size(const BotSet* set);

gboolean bot_set_contains(const BotSet* set, gpointer element);

typedef void (*BotSetForeachFunc)(gpointer element, gpointer user_data);
void bot_set_foreach(BotSet* set, BotSetForeachFunc func, gpointer user_data);

GPtrArray* bot_set_get_elements(BotSet* set);

#ifdef __cplusplus
}  // extern "C"
#endif

/**
 * @}
 */

#endif  // BOT2_CORE_BOT_CORE_SET_H_
