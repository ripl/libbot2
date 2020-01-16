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

#ifndef BOT2_CORE_BOT_CORE_GPS_LINEARIZE_H_
#define BOT2_CORE_BOT_CORE_GPS_LINEARIZE_H_

/**
 * @defgroup BotCoreGPSLinearize GPS Linearization
 * @ingroup BotCoreMathGeom
 * @brief Linearizing GPS coordinates
 * @include: bot_core/bot_core.h
 *
 * Linking: `pkg-config --libs bot2-core`
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BotGPSLinearize {
  double lon0_deg, lat0_deg;
  double radius_ns, radius_ew;
} BotGPSLinearize;

void bot_gps_linearize_init(BotGPSLinearize* gl, const double ll_deg[2]);
int bot_gps_linearize_to_xy(BotGPSLinearize* gl, const double ll_deg[2],
                            double xy[2]);
int bot_gps_linearize_to_lat_lon(BotGPSLinearize* gl, const double xy[2],
                                 double ll_deg[2]);

/**
 * @}
 */

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_CORE_BOT_CORE_GPS_LINEARIZE_H_
