// -*- mode: c -*-
// vim: set filetype=c :

/*
 * This file is part of bot2-param.
 *
 * bot2-param is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-param is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-param. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_PARAM_PARAM_SERVER_LCM_UTIL_H_
#define BOT2_PARAM_PARAM_SERVER_LCM_UTIL_H_

#include <lcm/lcm.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * lcu_mainloop_attach_lc (lc_t *lc)
 * attaches/detaches LC to/from the glib mainloop
 * When attached, lc_handle() is invoked "automatically" when a message is
 * received over LC.
 *
 * only one instance of lc_t can be attached per process
 *
 * returns 0 on success, -1 on failure
 */
int lcmu_glib_mainloop_attach_lcm(lcm_t* lcm);

int lcmu_glib_mainloop_detach_lcm(lcm_t* lcm);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_PARAM_PARAM_SERVER_LCM_UTIL_H_
