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

#ifndef BOT2_CORE_BOT_CORE_MATH_UTIL_H_
#define BOT2_CORE_BOT_CORE_MATH_UTIL_H_

/**
 * @defgroup BotCoreMathUtil Math Utilities
 * @ingroup BotCoreMathGeom
 * @brief Miscellaneous math utility functions
 * @include: bot_core/bot_core.h
 *
 * Linking: `pkg-config --libs bot2-core`
 *
 * @{
 */

#include <assert.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/** valid only for v > 0 **/
static inline double bot_mod2pi_positive(double vin)
{
    double q = vin / (2*M_PI) + 0.5;
    int qi = (int) q;

    return vin - qi*2*M_PI;
}

/** Map v to [-PI, PI] **/
static inline double bot_mod2pi(double vin)
{
    if (vin < 0)
        return -bot_mod2pi_positive(-vin);
    else
        return bot_mod2pi_positive(vin);
}

/** Return vin such that it is within PI degrees of ref **/
static inline double bot_mod2pi_ref(double ref, double vin)
{
    return ref + bot_mod2pi(vin - ref);
}

static inline int bot_theta_to_int(double theta, int max)
{
    theta = bot_mod2pi_ref(M_PI, theta);
    int v = (int) (theta / ( 2 * M_PI ) * max);

    if (v==max)
        v = 0;

    assert (v >= 0 && v < max);

    return v;
}

/** Convenience function for systems without the sincos GNU extension **/
static inline void bot_sincos(double theta, double *s, double *c)
{
    *s = sin(theta);
    *c = cos(theta);
}

#define bot_to_radians(deg) ((deg)*M_PI/180)

#define bot_to_degrees(rad) ((rad)*180/M_PI)

#define bot_sq(a) ((a)*(a))

#define bot_clamp(x,min,max) (x < min ? min : (x > max ? max : x))

#define bot_sgn(v)((v>=0) ? 1 : -1)

#define bot_min(a,b)((a < b) ? a : b)
#define bot_max(a,b)((a > b) ? a : b)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* BOT2_CORE_BOT_CORE_MATH_UTIL_H_ */
