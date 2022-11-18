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

#ifndef BOT2_CORE_BOT_CORE_SSOCKET_H_
#define BOT2_CORE_BOT_CORE_SSOCKET_H_

/**
 * @defgroup BotCoreSSocket TCP Sockets
 * @brief Convenience data structure for using a TCP socket
 * @ingroup BotCoreIO
 * @include: bot_core/bot_core.h
 *
 * Linking: `pkg-config --libs bot2-core`
 *
 * @{
 */

#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bot_ssocket {
  int type;
  int socket;

  struct sockaddr addr;
  socklen_t addrlen;
} bot_ssocket_t;

bot_ssocket_t* bot_ssocket_create(void);
void bot_ssocket_destroy(bot_ssocket_t* s);
int bot_ssocket_connect(bot_ssocket_t* s, const char* hostname, int port);
int bot_ssocket_disable_nagle(bot_ssocket_t* s);
int bot_ssocket_listen(bot_ssocket_t* s, int port, int listenqueue,
                       int localhostOnly);
bot_ssocket_t* bot_ssocket_accept(bot_ssocket_t* s);
void bot_ssocket_get_remote_ip(bot_ssocket_t* s, int* ip);
int bot_ssocket_get_fd(bot_ssocket_t* s);

#ifdef __cplusplus
}  // extern "C"
#endif

/**
 * @}
 */

#endif  // BOT2_CORE_BOT_CORE_SSOCKET_H_
