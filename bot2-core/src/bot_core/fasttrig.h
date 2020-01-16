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

#ifndef BOT2_CORE_BOT_CORE_FASTTRIG_H_
#define BOT2_CORE_BOT_CORE_FASTTRIG_H_

/**
 * @defgroup BotCoreFastTrig Fast Trigonometry
 * @ingroup BotCoreMathGeom
 * @brief Very fast, but approximate trigonometry
 * @include: bot_core/bot_core.h
 *
 * Linking: `pkg-config --libs bot2-core`
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

void bot_fasttrig_init(void);
void bot_fasttrig_sincos(double theta, double* s, double* c);
double bot_fasttrig_atan2(double y, double x);

/**
 * @}
 */

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_CORE_BOT_CORE_FASTTRIG_H_
