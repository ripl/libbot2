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

#ifndef BOT2_LCM_UTILS_TUNNEL_INTROSPECT_H_
#define BOT2_LCM_UTILS_TUNNEL_INTROSPECT_H_

#include <lcm/lcm.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _introspect_t introspect_t;

/**
 * introspection only works for udpm LCM networks
 */
introspect_t* introspect_new(lcm_t* lcm);

void introspect_send_introspection_packet(introspect_t* ipi);

void introspect_destroy(introspect_t* ipi);

int introspect_is_message_from_self(introspect_t* ipi,
                                    const lcm_recv_buf_t* rbuf,
                                    const char* channel);

int introspect_is_ready(const introspect_t* ipi);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_LCM_UTILS_TUNNEL_INTROSPECT_H_
