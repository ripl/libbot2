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

#ifndef BOT2_CORE_BOT_CORE_BOT_CORE_H_
#define BOT2_CORE_BOT_CORE_BOT_CORE_H_

// IWYU pragma: begin_exports
#include "camtrans.h"
#include "circular.h"
#include "color_util.h"
#include "ctrans.h"
#include "fasttrig.h"
#include "fileutils.h"
#include "glib_util.h"
#include "gps_linearize.h"
#include "lcm_util.h"
#include "math_util.h"
#include "minheap.h"
#include "ppm.h"
#include "ptr_circular.h"
#include "rand_util.h"
#include "ringbuf.h"
#include "rotations.h"
#include "serial.h"
#include "set.h"
#include "signal_pipe.h"
#include "small_linalg.h"
#include "ssocket.h"
#include "tictoc.h"
#include "timespec.h"
#include "timestamp.h"
#include "trans.h"
// IWYU pragma: end_exports

#include "lcmtypes/bot_core_image_metadata_t.h"  // IWYU pragma: keep
#include "lcmtypes/bot_core_image_sync_t.h"  // IWYU pragma: keep
#include "lcmtypes/bot_core_image_t.h"  // IWYU pragma: keep
#include "lcmtypes/bot_core_planar_lidar_t.h"  // IWYU pragma: keep
#include "lcmtypes/bot_core_pose_t.h"  // IWYU pragma: keep
#include "lcmtypes/bot_core_raw_t.h"  // IWYU pragma: keep
#include "lcmtypes/bot_core_rigid_transform_t.h"  // IWYU pragma: keep
#include "lcmtypes/bot_core_sensor_status_t.h"  // IWYU pragma: keep

/**
 * @defgroup BotCore Bot Core
 * @brief Core data structures and functions
 */

/**
 * @defgroup BotCoreMathGeom Math / Geometry
 * @ingroup BotCore
 * @brief Functions and data structures for math and geometry
 */

/**
 * @defgroup BotCoreDataStructures Data Structures
 * @ingroup BotCore
 * @brief General purpose data structures
 */

/**
 * @defgroup BotCoreIO I/O
 * @ingroup BotCore
 * @brief I/O routines (files, sockets, etc.) and reading/writing certain file
 * formats.
 */

/**
 * @defgroup BotCoreTime Time
 * @ingroup BotCore
 * @brief Time keeping
 */

#endif  // BOT2_CORE_BOT_CORE_BOT_CORE_H_
