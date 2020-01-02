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

#ifndef BOT2_LCM_UTILS_TUNNEL_LCM_TUNNEL_SERVER_H_
#define BOT2_LCM_UTILS_TUNNEL_LCM_TUNNEL_SERVER_H_

#include <list>

#include <glib.h>
#include <lcm/lcm.h>

#include "introspect.h"
#include "lcm_tunnel.h"
#include "ssocket.h"

class LcmTunnelServer {
public:
  static int initializeServer(tunnel_server_params_t * params);
  static void destroyServer();

  static bool initialized;

  static int acceptClient(GIOChannel *source, GIOCondition cond, void *user_data);
  static int disconnectClient(LcmTunnel * client);

  static GMainLoop * mainloop;
  static lcm_t * lcm;
  static introspect_t * introspect;

  static bool matches_a_client(const char *channel);
  static void check_and_send_to_tunnels(const char *channel,
      const void *data, unsigned int len, LcmTunnel * to_skip);

  static std::list<LcmTunnel *> clients_list;

  static ssocket_t * server_sock;
  static GIOChannel * server_sock_ioc;
  static guint server_sock_sid;

  static tunnel_server_params_t params;
};

#endif  // BOT2_LCM_UTILS_TUNNEL_LCM_TUNNEL_SERVER_H_
