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

#include "lcm_tunnel_server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <memory>  // IWYU pragma: keep

#include <lcm/lcm.h>

#include "lcm_tunnel.h"
#include "lcm_util.h"
#include "signal_pipe.h"
#include "ssocket.h"

bool LcmTunnelServer::initialized = false;
GMainLoop* LcmTunnelServer::mainloop;
lcm_t* LcmTunnelServer::lcm;
introspect_t* LcmTunnelServer::introspect;

std::list<LcmTunnel*> LcmTunnelServer::clients_list;

ssocket_t* LcmTunnelServer::server_sock;
GIOChannel* LcmTunnelServer::server_sock_ioc;
guint LcmTunnelServer::server_sock_sid;

tunnel_server_params_t LcmTunnelServer::params;

bool LcmTunnelServer::matches_a_client(const char* channel) {
  for (std::list<LcmTunnel*>::iterator iter =
           LcmTunnelServer::clients_list.begin();
       iter != clients_list.end(); iter++) {
    if ((*iter)->match_regex(channel)) {
      return true;
    }
  }
  return false;
}

void LcmTunnelServer::check_and_send_to_tunnels(const char* channel,
                                                const void* data,
                                                unsigned int len,
                                                LcmTunnel* to_skip) {
  for (std::list<LcmTunnel*>::iterator iter =
           LcmTunnelServer::clients_list.begin();
       iter != clients_list.end(); iter++) {
    if (*iter == to_skip) {
      continue;
    }
    if ((*iter)->match_regex(channel)) {
      (*iter)->send_to_remote(data, len, channel);
    }
  }
}

int LcmTunnelServer::initializeServer(tunnel_server_params_t* params_) {
  params = *params_;
  if (initialized) {
    if (params.verbose) {
      printf(
          "server already initialized... restarting everything with the new "
          "params\n");
    }
    destroyServer();
  }
  initialized = true;
  mainloop = g_main_loop_new(NULL, FALSE);
  bot_signal_pipe_glib_quit_on_kill(mainloop);

  // setup listening socket
  server_sock = ssocket_create();
  if (0 != ssocket_listen(server_sock, params.port, 1, 0)) {
    perror("creating server socket");
    return 0;
  }
  server_sock_ioc = g_io_channel_unix_new(ssocket_get_fd(server_sock));
  server_sock_sid =
      g_io_add_watch(server_sock_ioc, G_IO_IN, acceptClient, NULL);
  if (params.verbose) {
    printf("listening on port %d\n", params.port);
  }

  // setup LCM
  if (strlen(params.lcm_url)) {
    lcm = lcm_create(params.lcm_url);
  } else {
    lcm = lcm_create(NULL);
  }
  introspect = introspect_new(lcm);
  while (introspect_is_ready(introspect)) {
    lcm_handle(lcm);
  }

  bot_glib_mainloop_attach_lcm(lcm);
  return 1;
}

void LcmTunnelServer::destroyServer() {
  // cleanup
  std::list<LcmTunnel*>::iterator it;
  for (it = clients_list.begin(); it != clients_list.end(); ++it) {
    delete (*it);
  }
  clients_list.clear();

  ssocket_destroy(server_sock);
  introspect_destroy(introspect);
  lcm_destroy(lcm);
  g_main_loop_quit(mainloop);
  exit(0);
}

int LcmTunnelServer::acceptClient(GIOChannel* source, GIOCondition cond,
                                  void* user_data) {
  ssocket_t* client_sock = ssocket_accept(server_sock);
  if (!client_sock) {
    return TRUE;
  }
  LcmTunnel* tunnel_client = new LcmTunnel(params.verbose, NULL);
  if (tunnel_client->connectToClient(lcm, introspect, mainloop, client_sock,
                                     &params)) {
    clients_list.push_back(tunnel_client);
  } else {
    delete tunnel_client;
  }

  return TRUE;
}

int LcmTunnelServer::disconnectClient(LcmTunnel* client) {
  clients_list.remove(client);
  fprintf(stderr, "disconnecting client: %s\n", client->name);
  delete client;
  if (params.startedAsClient && clients_list.size() == 0) {
    fprintf(stderr, "All clients disconnected, exiting\n");
    destroyServer();
  }
  return 0;
}
