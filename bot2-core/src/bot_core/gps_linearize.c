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

// gps_linearize.c:
//
// Functions for projecting GPS coordinates into a planar coordinate system
// centered at a particular GPS point.  This is suitable for geometry
// computations using GPS in local neighborhoods of a few kilometers.

#include "gps_linearize.h"

#include <math.h>

#define REQ 6378135.0
#define RPO 6356750.0

// Useful links:
// http://www.movable-type.co.uk/scripts/LatLongVincenty.html
// http://en.wikipedia.org/wiki/Earth_radius

#define TO_RAD(x) ((x)*M_PI / 180.0)
#define TO_DEG(x) ((x)*180.0 / M_PI)
#define SQ(x) ((x) * (x))

void bot_gps_linearize_init(BotGPSLinearize* gl, const double ll_deg[2]) {
  gl->lat0_deg = ll_deg[0];
  gl->lon0_deg = ll_deg[1];

  double lat_rad = TO_RAD(ll_deg[0]);

  // best radius approximation in ns and ew direction.
  gl->radius_ns = SQ(REQ * RPO) /
      pow((SQ(REQ * cos(lat_rad))) + SQ(RPO * sin(lat_rad)), 1.5);
  gl->radius_ew =
      REQ * REQ / sqrt(SQ(REQ * cos(lat_rad)) + SQ(RPO * sin(lat_rad)));
}

int bot_gps_linearize_to_xy(BotGPSLinearize* gl, const double ll_deg[2],
                            double xy[2]) {
  double dlat = TO_RAD(ll_deg[0] - gl->lat0_deg);
  double dlon = TO_RAD(ll_deg[1] - gl->lon0_deg);

  xy[0] = sin(dlon) * gl->radius_ew * cos(TO_RAD(gl->lat0_deg));
  xy[1] = sin(dlat) * gl->radius_ns;

  return 0;
}

int bot_gps_linearize_to_lat_lon(BotGPSLinearize* gl, const double xy[2],
                                 double ll_deg[2]) {
  double dlat = asin(xy[1] / gl->radius_ns);
  ll_deg[0] = TO_DEG(dlat) + gl->lat0_deg;

  double dlon = asin(xy[0] / gl->radius_ew / cos(TO_RAD(gl->lat0_deg)));
  ll_deg[1] = TO_DEG(dlon) + gl->lon0_deg;

  return 0;
}
