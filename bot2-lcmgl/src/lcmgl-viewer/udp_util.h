// -*- mode: c -*-
// vim: set filetype=c :

/*
 * This file is part of bot2-lcmgl.
 *
 * bot2-lcmgl is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-lcmgl is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-lcmgl. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_LCMGL_LCMGL_VIEWER_UDP_UTIL_H_
#define BOT2_LCMGL_LCMGL_VIEWER_UDP_UTIL_H_

#include <string.h>  // IWYU pragma: keep

#ifdef __cplusplus
extern "C" {
#endif

/** make and bind a udp socket to an ephemeral port. Returns the fd. **/
int udp_socket_create(void);

/** make and bind a udp socket to a specified port. Returns the fd. **/
int udp_socket_listen(int port);

/** return the local port number for a socket. **/
int udp_socket_get_port(int sock);

// convenience method that sends a one-off udp message
// return 0 on success
int udp_send(const char* ipaddr, int port, const void* data, int datalen);

#define udp_send_string(ipaddr, port, string) \
  udp_send(ipaddr, port, string, strlen(string))

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_LCMGL_LCMGL_VIEWER_UDP_UTIL_H_
