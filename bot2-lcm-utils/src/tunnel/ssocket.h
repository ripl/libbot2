/*
 * This file is part of bot2-lcm-utils.
 *
 * bot2-lcm-utils is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-lcm-utils is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-lcm-utils. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_LCM_UTILS_TUNNEL_SSOCKET_H_
#define BOT2_LCM_UTILS_TUNNEL_SSOCKET_H_

#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ssocket
{
	int type;
	int socket;

	struct sockaddr addr;
	socklen_t addrlen;
} ssocket_t;

ssocket_t *ssocket_create(void);
void ssocket_destroy(ssocket_t *s);

// returns < 0 on error
int ssocket_connect(ssocket_t *s, const char *hostname, int port);

int ssocket_disable_nagle(ssocket_t *s);
int ssocket_listen(ssocket_t *s, int port, int listenqueue, int localhostOnly);
ssocket_t *ssocket_accept(ssocket_t *s);

/**
 * ssocket_get_remote_ip:
 *
 * retrieves the IP address of the remote end of the connection.  each octet of
 * the address is stored in a separate array element
 */
void ssocket_get_remote_ip(ssocket_t *s, int ip[4]);

int ssocket_get_fd(ssocket_t *s);

#ifdef __cplusplus
}
#endif

#endif  /* BOT2_LCM_UTILS_TUNNEL_SSOCKET_H_ */
