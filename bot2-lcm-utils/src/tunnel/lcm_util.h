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

#ifndef BOT2_LCM_UTILS_TUNNEL_LCM_UTIL_H_
#define BOT2_LCM_UTILS_TUNNEL_LCM_UTIL_H_

// lcm_util was copied from bot_core to make lcm_tunnel a standalone pod

#include <glib.h>
#include <lcm/lcm.h>

/**
 * @defgroup BotCoreLcmUtil LcmUtil
 * @ingroup BotCoreIO
 * @brief Convenience functions for working with LCM
 * @include: bot_core/bot_core.h
 *
 * These functions attach an #lcm_t object to a #GMainLoop so that when there is
 * a new message on LCM, lcm_handle() gets called, and your message handlers
 * get invoked.
 *
 * Linking: `pkg-config --libs bot2-core`
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * bot_glib_mainloop_attach_lcm:
 * @lcm: The LCM object to attach to the default #GMainLoop.
 *
 * Attaches LCM to the default #GMainLoop. When attached, lcm_handle() is
 * invoked "automatically" when a message is received over LCM.
 *
 * Only one instance of #lcm_t can be attached per process.
 *
 * This is the same as calling
 * bot_glib_mainloop_attach_lcm_full(%NULL, @lcm, %NULL, %NULL, %NULL).
 *
 * Returns: 0 on success, -1 on failure
 */
int bot_glib_mainloop_attach_lcm(lcm_t* lcm);

/**
 * bot_glib_mainloop_detach_lcm:
 * @lcm: The LCM object to detach from whichever #GMainLoop it was attached
 * to.
 *
 * Detaches the passed LCM instance from whichever #GMainLoop it was attached
 * to.
 *
 * Returns: 0 on success, -1 on failure.
 */
int bot_glib_mainloop_detach_lcm(lcm_t* lcm);

/**
 * bot_glib_mainloop_attach_lcm_full:
 * @mainloop: The mainloop to attach to or %NULL if default.
 * @lcm: The #lcm_t object to attach.
 * @quit_on_lcm_fail:  If %TRUE, calls g_main_loop_quit() when lcm_handle()
 * returns an error.
 *
 * Attaches the passed LCM object to the passed #GMainLoop (or the default one
 * if the passed value is %NULL).
 *
 * Returns: 0 on success, -1 on failure.
 */
int bot_glib_mainloop_attach_lcm_full(GMainLoop* mainloop, lcm_t* lcm,
                                      gboolean quit_on_lcm_fail);

/**
 * bot_lcm_get_global:
 * @provider: The string specifying the LCM network provider. If %NULL, the
 * environment variable "LCM_DEFAULT_URL" is used if it is defined, otherwise
 * the lcm_create default settings are used.
 *
 * The first time this function is invoked, it instantiates and returns new
 * lcm_t instance via lcm_create(provider). Every subsequent call just returns
 * the same lcm_t instance.
 *
 * This function is thread-safe.
 *
 * Returns: pointer to lcm_t
 */
lcm_t* bot_lcm_get_global(const char* provider);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_LCM_UTILS_TUNNEL_LCM_UTIL_H_
