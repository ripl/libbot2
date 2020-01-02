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

#ifndef BOT2_CORE_BOT_CORE_MINHEAP_H_
#define BOT2_CORE_BOT_CORE_MINHEAP_H_

#include <glib.h>

/**
 * @defgroup BotCoreMinHeap Minheap
 * @brief Heap data structure
 * @ingroup BotCoreDataStructures
 * @include: bot_core/bot_core.h
 *
 * Linking: `pkg-config --libs bot2-core`
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BotMinheap BotMinheap;
typedef struct _BotMinheapNode BotMinheapNode;

BotMinheap *bot_minheap_new(void);

BotMinheap *bot_minheap_sized_new(int capacity);

void bot_minheap_free(BotMinheap *mh);

BotMinheapNode *bot_minheap_add (BotMinheap *mh, void *data, double score);

void bot_minheap_decrease_score (BotMinheap *mh, BotMinheapNode *node,
        double score);

void *bot_minheap_remove_min (BotMinheap *mh, double *score);

int bot_minheap_size (BotMinheap *mh);

gboolean bot_minheap_is_empty (BotMinheap *mh);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  /* BOT2_CORE_BOT_CORE_MINHEAP_H_ */
