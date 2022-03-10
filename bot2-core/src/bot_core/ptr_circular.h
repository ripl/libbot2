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

#ifndef BOT2_CORE_BOT_CORE_PTR_CIRCULAR_H_
#define BOT2_CORE_BOT_CORE_PTR_CIRCULAR_H_

/**
 * @defgroup BotCorePtrCircular Circular Pointer Array
 * @brief Circular array of pointers
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

typedef void (*BotPtrCircularDestroy)(void* user, void* p);

/** A circular Buffer, implemented with a PtrArray **/
typedef struct gcircular {
  void** p;  // storage
  unsigned int size;  // number of valid elements in the buffer
  unsigned int next;  // where the next add will go
  unsigned int capacity;  // maximum allowed capacity

  void* user;
  BotPtrCircularDestroy handler;
} BotPtrCircular;

// create a new circular buffer; the destroy handler will be called
// whenever an element is evicted. (NULL means no handler).
BotPtrCircular* bot_ptr_circular_new(unsigned int capacity,
                                     BotPtrCircularDestroy handler, void* user);

// call destroy() on any elements, then deallocate the circular buffer
void bot_ptr_circular_destroy(BotPtrCircular* circ);

// adds a new element to the buffer, possibly evicting the oldest
void bot_ptr_circular_add(BotPtrCircular* circ, void* p);

// return the number of valid elements in the buffer
unsigned int bot_ptr_circular_size(BotPtrCircular* circ);

// An index of zero corresponds to the most recently added item.
void* bot_ptr_circular_index(BotPtrCircular* circ, unsigned int idx);

// resize the circular buffer, freeing elements as required
void bot_ptr_circular_resize(BotPtrCircular* circ, unsigned int capacity);

// remove all elements from the buffer.
void bot_ptr_circular_clear(BotPtrCircular* circ);

#ifdef __cplusplus
}  // extern "C"
#endif

/**
 * @}
 */

#endif  // BOT2_CORE_BOT_CORE_PTR_CIRCULAR_H_
