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

#ifndef BOT2_CORE_BOT_CORE_CIRCULAR_H_
#define BOT2_CORE_BOT_CORE_CIRCULAR_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup BotCoreCircular Circular Array
 * @brief Fixed-capacity circular array
 * @ingroup BotCoreDataStructures
 * @include: bot_core/bot_core.h
 *
 * BotCircular is a hybrid of the glib types GArray and GQueue.  BotCircular
 * acts like a GQueue in the sense that you can push on one end and pop
 * from the other.  It acts like a GArray in the sense that its contents
 * are statically allocated rather than pointers to user-allocated buffers.
 * For this reason, its size is fixed and allocated when the BotCircular
 * is created (TODO(ashuang): set_size function).  If a new element is pushed
 * when the BotCircular is already full, the last element on the tail is
 * automatically overwritten.
 *
 * Linking: `pkg-config --libs bot2-core`
 *
 * @{
 */
typedef struct _BotCircular {
  int len;
  int capacity;
  int element_size;
  int head;
  void* array;
} BotCircular;

BotCircular* bot_circular_new(int capacity, int element_size);
void bot_circular_free(BotCircular* circular);
void bot_circular_clear(BotCircular* circular);
int bot_circular_push_head(BotCircular* circular, const void* data);
int bot_circular_pop_tail(BotCircular* circular, void* data);
int bot_circular_pop_head(BotCircular* circular, void* data);

/**
 * bot_circular_size:
 * Returns: the number of valid elements.
 */
int bot_circular_size(BotCircular* circular);

#define bot_circular_is_empty(a) ((a)->len == 0)

#define bot_circular_is_full(a) ((a)->len >= (a)->capacity)

#define bot_circular_peek_nth(a, i) \
  ((void*)((char*)(a)->array +      \
           (((a)->head + (i)) % (a)->capacity) * (a)->element_size))

/**
 * @}
 */

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_CORE_BOT_CORE_CIRCULAR_H_
