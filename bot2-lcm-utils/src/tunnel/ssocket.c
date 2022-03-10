// -*- mode: c -*-
// vim: set filetype=c :

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

#include "ssocket.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define SSOCKET_UNKNOWN_TYPE 0
#define SSOCKET_SERVER_TYPE 1
#define SSOCKET_CLIENT_TYPE 2

ssocket_t* ssocket_create() {
  ssocket_t* s = (ssocket_t*)calloc(1, sizeof(ssocket_t));
  s->socket = -1;

  return s;
}

void ssocket_destroy(ssocket_t* s) {
  if (s->socket >= 0) {
    close(s->socket);
  }
  free(s);
}

int ssocket_connect(ssocket_t* s, const char* hostname, int port) {
  struct hostent* host = NULL;
  struct sockaddr_in sa;
  struct sockaddr_in ca;
  int thesocket;

  // let's find out about this host
  int host_lookup = 0;
  if (0 == inet_aton(hostname, &ca.sin_addr)) {
    host_lookup = 1;
    host = gethostbyname(hostname);
    if (host == NULL) {
      return -1;
    }
  }

  // create the socket
  thesocket = socket(AF_INET, SOCK_STREAM, 0);

  // fill in the fields
  bzero(&sa, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(0);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  // bind it to the port
  if (bind(thesocket, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
    close(thesocket);
    return -1;
  }

  sa.sin_port = htons(port);
  if (host_lookup) {
    sa.sin_addr = *(struct in_addr*)host->h_addr;
  } else {
    sa.sin_addr = ca.sin_addr;
  }

  if (connect(thesocket, (struct sockaddr*)&sa, sizeof(sa))) {
    if (errno != EINPROGRESS) {
      close(thesocket);
      return -1;
    }
  }

  s->type = SSOCKET_CLIENT_TYPE;
  s->socket = thesocket;

  // prevent "broken pipe" signals.
  signal(SIGPIPE, SIG_IGN);

  return 0;
}

// returns 0 on success
int ssocket_disable_nagle(ssocket_t* s) {
  int n = 1;

  if (setsockopt(s->socket, IPPROTO_TCP, TCP_NODELAY, (char*)&n, sizeof(n)) <
      0) {
    perror("could not setsockopt");
    close(s->socket);
    return -1;
  }

  return 0;
}

int ssocket_listen(ssocket_t* s, int port, int listenqueue, int localhostOnly) {
  int thesocket;
  int n;
  struct sockaddr_in sa;

  thesocket = socket(AF_INET, SOCK_STREAM, 0);
  if (thesocket == -1) {
    return -1;
  }

  // avoid address already in use errors
  n = 1;
  if (setsockopt(thesocket, SOL_SOCKET, SO_REUSEADDR, (char*)&n, sizeof(n)) <
      0) {
    close(thesocket);
    return -1;
  }

  // fill in the fields
  bzero(&sa, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = htonl(localhostOnly ? INADDR_LOOPBACK : INADDR_ANY);

  // try to bind to the port

  if (bind(thesocket, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
    close(thesocket);
    return -1;
  }

  // start listening
  if (listen(thesocket, listenqueue)) {
    close(thesocket);
    perror("listen");
    return -1;
  }

  s->type = SSOCKET_SERVER_TYPE;
  s->socket = thesocket;

  return 0;
}

ssocket_t* ssocket_accept(ssocket_t* s) {
  int thesocket;
  ssocket_t* cs = ssocket_create();

  s->addrlen = sizeof(struct sockaddr);

  if (s->type != SSOCKET_SERVER_TYPE) {
    printf("not server type\n");
    return NULL;
  }

  // actually accept the connection
  if ((thesocket = accept(s->socket, &s->addr, &s->addrlen)) < 0) {
    perror("accept failed");
    return NULL;
  }

  cs->type = SSOCKET_CLIENT_TYPE;
  cs->socket = thesocket;

  return cs;
}

void ssocket_get_remote_ip(ssocket_t* s, int* ip) {
  struct sockaddr addr;
  socklen_t addrlen;

  getpeername(s->socket, &addr, &addrlen);

  ip[0] = (int)(addr.sa_data[2]) & 0xff;
  ip[1] = (int)(addr.sa_data[3]) & 0xff;
  ip[2] = (int)(addr.sa_data[4]) & 0xff;
  ip[3] = (int)(addr.sa_data[5]) & 0xff;
}

int ssocket_get_fd(ssocket_t* s) { return s->socket; }
