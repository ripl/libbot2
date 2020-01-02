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

#ifndef BOT2_CORE_BOT_CORE_RINGBUF_H_
#define BOT2_CORE_BOT_CORE_RINGBUF_H_

#include <stdint.h>

/**
 * @defgroup BotCoreRingbuf A simple ring buffer
 * @brief A fixed capacity ring buffer
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

/**
 *
 *
 */
typedef struct _BotRingBuf BotRingBuf;

/*
 * Create buffer, and allocate space for size bytes
 */
BotRingBuf * bot_ringbuf_create(int size);

/*
 * Destroy it
 */
void bot_ringbuf_destroy(BotRingBuf * cbuf);

/*
 * Copy numBytes from the head of the buffer, and move read pointers
 */
int bot_ringbuf_read(BotRingBuf * cbuf, int numBytes, uint8_t * buf);

/*
 * Copy numBytes from buf to end of buffer
 */
int bot_ringbuf_write(BotRingBuf * cbuf, int numBytes, uint8_t * buf);

/*
 * Fill the ringbuff with data from the file descriptor.
 * Either read numBytes from the fd,
 * or if numBytes<0, get all available bytes
 */
int bot_ringbuf_fill_from_fd(BotRingBuf * cbuf, int fd, int numBytes);

/*
 * Copy numBytes from the head of the buffer, but DON'T move read pointers
 */
int bot_ringbuf_peek(BotRingBuf * cbuf, int numBytes, uint8_t * buf); //read numBytes from start of buffer, but don't move readPtr

/**
 * Return a pointer to a contiguous buffer with the next numBytes of data to be read
 */
const uint8_t * bot_ringbuf_peek_buf(BotRingBuf * cbuf, int numBytes);

/*
 * flush numBytes from the buffer.
 * pass in -1 to empy buffer completely!
 */
int bot_ringbuf_flush(BotRingBuf * cbuf, int numBytes);
/*
 * Get the amount of data currently stored in the buffer (not the allocated size)
 */
int bot_ringbuf_available(BotRingBuf * cbuf);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  /* BOT2_CORE_BOT_CORE_RINGBUF_H_ */
