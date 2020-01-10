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

/** Simplified serial utilities. As opposed to my earlier
    serial class, we let you use the fd (or FILE*) directly.

    eolson@mit.edu, 2004
**/

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#ifdef __linux__
#include <linux/serial.h>
#include <linux/tty_flags.h>

#define SUPPORT_HISPEED 1
#endif

#include "serial.h"

static int bot_serial_translate_baud(int inrate);

/** Creates a basic fd, setting baud to 9600, raw data i/o (no flow
    control, no fancy character handling. Configures it for blocking
    reads.

    Returns the fd, -1 on error
**/
int bot_serial_open(const char *port, int baud, int blocking)
{
	struct termios opts;

	int flags = O_RDWR | O_NOCTTY;
	if (!blocking)
		flags |= O_NONBLOCK;

	int fd=open(port, flags, 0);
	if (fd==-1)
		return -1;

	if (tcgetattr(fd, &opts))
	{
		printf("*** %i\n",fd);
		perror("tcgetattr");
		return -1;
	}

	cfsetispeed(&opts, bot_serial_translate_baud(baud));
	cfsetospeed(&opts, bot_serial_translate_baud(baud));

	cfmakeraw(&opts);

        // set one stop bit
        opts.c_cflag &= ~CSTOPB;

	if (tcsetattr(fd,TCSANOW,&opts))
	{
		perror("tcsetattr");
		return -1;
	}

	tcflush(fd, TCIOFLUSH);
	return fd;
}

int bot_serial_set_N82 (int fd)
{
    struct termios opts;

    if (tcgetattr(fd, &opts))
    {
        perror("tcgetattr");
        return -1;
    }

    opts.c_cflag &= ~CSIZE;
    opts.c_cflag |= CS8;
    opts.c_cflag |= CSTOPB;

    if (tcsetattr(fd,TCSANOW,&opts))
    {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

/** Enable cts/rts flow control.
    Returns non-zero on error.
**/
int bot_serial_enablectsrts(int fd)
{
	struct termios opts;

	if (tcgetattr(fd, &opts))
	{
		perror("tcgetattr");
		return -1;
	}

	opts.c_cflag |= CRTSCTS;

	if (tcsetattr(fd,TCSANOW,&opts))
	{
		perror("tcsetattr");
		return -1;
	}

	return 0;
}

/** Enable xon/xoff flow control.
    Returns non-zero on error.
**/
int bot_serial_enablexon(int fd)
{
	struct termios opts;

	if (tcgetattr(fd, &opts))
	{
		perror("tcgetattr");
		return -1;
	}

	opts.c_iflag |= (IXON | IXOFF);

	if (tcsetattr(fd,TCSANOW,&opts))
	{
		perror("tcsetattr");
		return -1;
	}

	return 0;
}

/** Set the baud rate, where the baudrate is just the integer value
    desired.

    Returns non-zero on error.
**/
int bot_serial_setbaud(int fd, int baudrate)
{
	struct termios tios;
#ifdef SUPPORT_HISPEED
	struct serial_struct ser;
#endif

	int baudratecode=bot_serial_translate_baud(baudrate);

	if (baudratecode>0)
	{
		tcgetattr(fd, &tios);
		cfsetispeed(&tios, baudratecode);
		cfsetospeed(&tios, baudratecode);
		tcflush(fd, TCIFLUSH);
		tcsetattr(fd, TCSANOW, &tios);

#ifdef SUPPORT_HISPEED
		ioctl(fd, TIOCGSERIAL, &ser);

		ser.flags=(ser.flags&(~ASYNC_SPD_MASK));
		ser.custom_divisor=0;

		ioctl(fd, TIOCSSERIAL, &ser);
#endif
	}
	else
	{
#ifdef SUPPORT_HISPEED
		if (tcgetattr(fd, &tios))
			perror("tcgetattr");

		cfsetispeed(&tios, B38400);
		cfsetospeed(&tios, B38400);
		tcflush(fd, TCIFLUSH);

		if (tcsetattr(fd, TCSANOW, &tios))
			perror("tcsetattr");

		if (ioctl(fd, TIOCGSERIAL, &ser))
			perror("ioctl TIOCGSERIAL");

		ser.flags=(ser.flags&(~ASYNC_SPD_MASK)) | ASYNC_SPD_CUST;
		ser.custom_divisor=48;
		ser.custom_divisor=ser.baud_base/baudrate;
		ser.reserved_char[0]=0; // what the hell does this do?

		if (ioctl(fd, TIOCSSERIAL, &ser))
			perror("ioctl TIOCSSERIAL");

#endif

	}

	tcflush(fd, TCIFLUSH);

	return 0;
}

// private function
int bot_serial_translate_baud(int inrate)
{
	switch(inrate)
	{
	case 0:
		return B0;
	case 300:
		return B300;
	case 1200:
		return B1200;
	case 2400:
		return B2400;
	case 4800:
		return B4800;
	case 9600:
		return B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
	case 57600:
		return B57600;
	case 115200:
		return B115200;
	case 230400:
		return B230400;
#ifdef SUPPORT_HISPEED
	case 460800:
		return B460800;
#endif
	default:
		return -1; // do custom divisor
	}
}

int bot_serial_close(int fd)
{
	return close(fd);
}

int bot_serial_bytes_available(int fd)
{
  //get number of bytes available
  int available = 0;
  if (ioctl(fd, FIONREAD, &available) != 0) {
    perror("Problem getting num bytes available.");
    return -1;
  }
  return available;
}
