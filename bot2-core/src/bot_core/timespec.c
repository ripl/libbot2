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

#include "timespec.h"
/* IWYU pragma: no_forward_declare timespec */
/* IWYU pragma: no_forward_declare timeval */

#include <stdio.h>

void bot_timespec_to_timeval(struct timespec *ts, struct timeval *tv)
{
  tv->tv_sec  = ts->tv_sec;
  tv->tv_usec = ts->tv_nsec / 1000;
}

void bot_timespec_now(struct timespec *ts)
{
	struct timeval  tv;

	// get the current time
	gettimeofday(&tv, NULL);
	ts->tv_sec  = tv.tv_sec;
	ts->tv_nsec = tv.tv_usec*1000;
}

void bot_timespec_addms(struct timespec *ts, long ms)
{
	int sec=ms/1000;
	ms=ms-sec*1000;

	// perform the addition
	ts->tv_nsec+=ms*1000000;

	// adjust the time
	ts->tv_sec+=ts->tv_nsec/1000000000 + sec;
	ts->tv_nsec=ts->tv_nsec%1000000000;
}

void bot_timespec_addns(struct timespec *ts, long ns)
{
	int sec=ns/1000000000;
	ns=ns - sec*1000000000;

	// perform the addition
	ts->tv_nsec+=ns;

	// adjust the time
	ts->tv_sec+=ts->tv_nsec/1000000000 + sec;
	ts->tv_nsec=ts->tv_nsec%1000000000;
}

void bot_timeval_set(struct timeval *tv, double dt)
{
	long us = dt*1000000;

	tv->tv_sec = us / 1000000;
	tv->tv_usec = us - tv->tv_sec*1000000;
}

void bot_timespec_adjust(struct timespec *ts, double dt)
{
	int sec;
	long ns;

	sec = (int) dt;
	ns = (dt - sec) * 1000000000;

	while (ns < 0) {
		ns += 1000000000;
		sec--;
	}

	// perform the addition
	ts->tv_nsec+=ns;

	// adjust the time
	ts->tv_sec+=ts->tv_nsec/1000000000 + sec;
	ts->tv_nsec=ts->tv_nsec%1000000000;
}

int bot_timespec_compare(struct timespec *a, struct timespec *b)
{
	if (a->tv_sec!=b->tv_sec)
		return a->tv_sec-b->tv_sec;

	return a->tv_nsec-b->tv_nsec;
}

// computes a = a-b
void bot_timespec_subtract(struct timespec *a, struct timespec *b)
{
	a->tv_nsec = a->tv_nsec - b->tv_nsec;
	if (a->tv_nsec < 0) {
		// borrow.
		a->tv_nsec += 1000000000;
		a->tv_sec --;
	}

	a->tv_sec = a->tv_sec - b->tv_sec;
}

// convert the timespec into milliseconds (may overflow)
int bot_timespec_milliseconds(struct timespec *a)
{
	return a->tv_sec*1000 + a->tv_nsec/1000000;
}

void bot_timespec_print(struct timespec *a)
{
	printf("%li.%09li\n",a->tv_sec, a->tv_nsec);
}
