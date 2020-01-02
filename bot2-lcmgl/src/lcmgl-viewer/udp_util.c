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

#include "udp_util.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/** make and bind a udp socket to a specified port. Returns the fd. **/
int udp_socket_listen(int port)
{
    int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
        return -1;

    struct sockaddr_in listen_addr;
    memset(&listen_addr, 0, sizeof(struct sockaddr_in));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(port);
    listen_addr.sin_addr.s_addr = INADDR_ANY;

    int res = bind(sock, (struct sockaddr*) &listen_addr, sizeof(struct sockaddr_in));
    if (res < 0)
        return -2;

    return sock;
}

/** make and bind a udp socket to an ephemeral port. Returns the fd. **/
int udp_socket_create(void)
{
    int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
        return -1;

    struct sockaddr_in listen_addr;
    memset(&listen_addr, 0, sizeof(struct sockaddr_in));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = INADDR_ANY; // ephemeral port, please
    listen_addr.sin_addr.s_addr = INADDR_ANY;

    int res = bind(sock, (struct sockaddr*) &listen_addr, sizeof(struct sockaddr_in));
    if (res < 0)
        return -2;

    return sock;
}

/** return the local port number for a socket. **/
int udp_socket_get_port(int sock)
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    int res = getsockname(sock, (struct sockaddr*) &addr, &addr_len);

    if (res < 0)
        return -1;

    return ntohs(addr.sin_port);
}

// return 0 on success
int udp_send(const char *ipaddr, int port, const void *data, int datalen)
{
    // fill in address structure
    struct sockaddr_in remote_addr;

    memset(&remote_addr, 0, sizeof(struct sockaddr_in));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(port);

    struct hostent *host = gethostbyname(ipaddr);
    if (host == NULL) {
        printf("Couldn't resolve host %s\n", ipaddr);
        return -1;
    }

    memcpy(&remote_addr.sin_addr.s_addr, host->h_addr, host->h_length);

    // now actually send.
    int sock = udp_socket_create();
    ssize_t res = sendto(sock, data, datalen, 0, (struct sockaddr*) &remote_addr, sizeof(remote_addr));

    // cleanup
    close(sock);
    return res != datalen;
}
