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

#ifndef BOT2_CORE_BOT_CORE_SERIAL_H_
#define BOT2_CORE_BOT_CORE_SERIAL_H_

/**
 * @defgroup BotCoreSerial Serial ports
 * @brief Reading and writing from serial ports
 * @ingroup BotCoreIO
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

/** Creates a basic fd, setting baud to 9600, raw data i/o (no flow
    control, no fancy character handling. Configures it for blocking
    reads.  8 data bits, 1 stop bit, no parity.

    Returns the fd, -1 on error
**/
int bot_serial_open(const char *port, int baud, int blocking);

/** Set the baud rate, where the baudrate is just the integer value
    desired.

    Returns non-zero on error.
**/
int bot_serial_setbaud(int fd, int baudrate);

/** Enable cts/rts flow control.
    Returns non-zero on error.
**/
int bot_serial_enablectsrts(int fd);
/** Enable xon/xoff flow control.
    Returns non-zero on error.
**/
int bot_serial_enablexon(int fd);

/** Set the port to 8 data bits, 2 stop bits, no parity.
    Returns non-zero on error.
 **/
int bot_serial_set_N82 (int fd);

int bot_serial_close(int fd);

/**
 * Get the number of bytes waiting to be read
 */
int bot_serial_bytes_available(int fd);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  /* BOT2_CORE_BOT_CORE_SERIAL_H_ */
