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

#include "ringbuf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serial.h"

#ifndef MIN
#define MIN(a,b)((a < b) ? a : b)
#endif

struct _BotRingBuf{
        uint8_t * buf;
        int readOffset;
        int writeOffset;
        int numBytes;
        int maxSize;
        uint8_t * read_buf;
        int read_buf_sz;
};

BotRingBuf * bot_ringbuf_create(int size)
{
  //create buffer, and allocate space for size bytes
  BotRingBuf * cbuf = (BotRingBuf *) calloc(1,sizeof(BotRingBuf));
  cbuf->buf = (uint8_t *) malloc(size * sizeof(uint8_t));
  cbuf->readOffset = cbuf->writeOffset = cbuf->numBytes = 0;
  cbuf->maxSize = size;

  cbuf->read_buf_sz = 256;
  cbuf->read_buf = (uint8_t *) malloc(cbuf->read_buf_sz * sizeof(uint8_t));

  return cbuf;
}

void bot_ringbuf_destroy(BotRingBuf * cbuf)
{
  //destroy
  if (cbuf->buf!=NULL)
    free(cbuf->buf);
  free(cbuf);
}

int bot_ringbuf_read(BotRingBuf * cbuf, int numBytes, uint8_t * buf)
{
  //read numBytes
  int bytes_read = bot_ringbuf_peek(cbuf, numBytes, buf);

  bot_ringbuf_flush(cbuf,bytes_read);

  return bytes_read;
}

int bot_ringbuf_write(BotRingBuf * cbuf, int numBytes, uint8_t * buf)
{
  //check if there is enough space... maybe this should just wrap around??
  if (numBytes + cbuf->numBytes > cbuf->maxSize) {
    fprintf(stderr, "CIRC_BUF ERROR: not enough space in circular buffer,Discarding data!\n");
    numBytes = cbuf->maxSize - cbuf->numBytes;

  }
  //write to wrap around point.
  int bytes_written = MIN(cbuf->maxSize - cbuf->writeOffset, numBytes);
  memcpy(cbuf->buf + cbuf->writeOffset, buf, bytes_written * sizeof(char));
  numBytes -= bytes_written;

  //write the rest from start of buffer
  if (numBytes > 0) {
    memcpy(cbuf->buf, buf + bytes_written, numBytes * sizeof(char));
    bytes_written += numBytes;
  }

  //move writePtr
  cbuf->numBytes += bytes_written;
  cbuf->writeOffset = (cbuf->writeOffset + bytes_written) % cbuf->maxSize;

  return bytes_written;
}

int bot_ringbuf_peek(BotRingBuf * cbuf, int numBytes, uint8_t * buf)
{
  //read numBytes from start of buffer, but don't move readPtr
  if (numBytes > cbuf->numBytes || numBytes > cbuf->maxSize) {
    fprintf(stderr, "CIRC_BUF ERROR: Can't read %d bytes from the circular buffer, only %d available! \n",
        numBytes,cbuf->numBytes);
    return -1;
  }
  //read up to wrap around point
  int bytes_read = MIN(cbuf->maxSize - cbuf->readOffset, numBytes);
  memcpy(buf, cbuf->buf + cbuf->readOffset, bytes_read * sizeof(char));
  numBytes -= bytes_read;

  //read again from beginning if there are bytes left
  if (numBytes > 0) {
    memcpy(buf + bytes_read, cbuf->buf, numBytes * sizeof(char));
    bytes_read += numBytes;
  }
  return bytes_read;
}

int bot_ringbuf_flush(BotRingBuf * cbuf, int numBytes)
{
  //move pointers to "empty" the read buffer
  if (numBytes<0)
    cbuf->readOffset = cbuf->writeOffset = cbuf->numBytes = 0;
  else{
    //move readPtr
    cbuf->numBytes -= numBytes;
    cbuf->readOffset = (cbuf->readOffset + numBytes) % cbuf->maxSize;
  }
  return 0;
}

int bot_ringbuf_available(BotRingBuf * cbuf) {
        return cbuf->numBytes;
}

int bot_ringbuf_fill_from_fd(BotRingBuf * cbuf, int fd, int numBytes)
{
  if (numBytes < 0) {
    numBytes = bot_serial_bytes_available(fd);
    if (numBytes <= 0)
      return numBytes;
  }

  //check if there is enough space... maybe this should just wrap around??
  if (numBytes + cbuf->numBytes > cbuf->maxSize) {
    numBytes = cbuf->maxSize - cbuf->numBytes;
  }
  //write to wrap around point.
  int bytes_written = MIN(cbuf->maxSize - cbuf->writeOffset, numBytes);
  int num_read = read(fd, cbuf->buf + cbuf->writeOffset, bytes_written);
  if (num_read != bytes_written) {
    fprintf(stderr, "warning, read %d of %d available bytes\n", num_read, bytes_written);
  }
  numBytes -= bytes_written;

  //write the rest from start of buffer
  if (numBytes > 0) {
    int num_read = read(fd, cbuf->buf, numBytes);
    if (num_read != numBytes) {
      fprintf(stderr, "warning, read %d of %d available bytes\n", num_read, numBytes);
    }
    bytes_written += numBytes;
  }

  //move writePtr
  cbuf->numBytes += bytes_written;
  cbuf->writeOffset = (cbuf->writeOffset + bytes_written) % cbuf->maxSize;

  return bytes_written;
}

const uint8_t * bot_ringbuf_peek_buf(BotRingBuf * cbuf, int numBytes)
{
  if (numBytes > cbuf->maxSize) {
    fprintf(stderr, "ERROR: can't read %d bytes from ringbuf, maxsize is %d\n", numBytes, cbuf->maxSize);
    return NULL;
  }
  else if (numBytes > cbuf->numBytes) {
    fprintf(stderr, "ERROR: can't read %d bytes from ringbuf, currently containts is %d\n", numBytes, cbuf->numBytes);
    return NULL;
  }

  int contiguous_bytes = cbuf->maxSize - cbuf->readOffset;
  if (numBytes < contiguous_bytes)
    return cbuf->buf + cbuf->readOffset;

  if (numBytes > cbuf->read_buf_sz) {
    cbuf->read_buf_sz = numBytes;
    cbuf->read_buf = (uint8_t *) realloc(cbuf->read_buf, cbuf->read_buf_sz * sizeof(uint8_t));
  }

  bot_ringbuf_peek(cbuf, numBytes, cbuf->read_buf);
  return cbuf->read_buf;
}
