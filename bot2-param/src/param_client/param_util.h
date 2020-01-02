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

#ifndef BOT2_PARAM_BOT_PARAM_PARAM_UTIL_H_
#define BOT2_PARAM_BOT_PARAM_PARAM_UTIL_H_

#include <bot_core/camtrans.h>
#include <bot_core/trans.h>

#include "param_client.h"

#ifdef __cplusplus
extern "C" {
#endif

  /*
   * Get a normalized BotTrans object from a calibration block that contains the subkeys:
   * translation
   * and a rotation specified as one of
   * quat, rpy (in degrees!), rodrigues, angleaxis
   */
  int bot_param_get_trans(BotParam * param, const char *name, BotTrans *trans);

  //TODO: should these be public?
  /*
   * Get the translation subcomponent of the block "name"
   */
  int bot_param_get_translation(BotParam *param, const char *name, double translation[3]);

  /*
   * Get the rotation subcomponent of the block "name". Rotation is specified as one of
   * quat, rpy (in degrees!), rodrigues, angleaxis
   */
  int bot_param_get_quat(BotParam * param, const char *name, double quat[4]);

  // Convenience Functions for lidars/cameras
  /*
   * Get the list of all cameras/lidars
   */
  char** bot_param_get_all_camera_names(BotParam *param);
  char** bot_param_get_all_planar_lidar_names(BotParam *param);

  /*
   * Get the prefix in the config file for the camera/lidar information
   */
  int bot_param_get_camera_prefix(BotParam *param, const char *cam_name, char *result, int result_size);
  int bot_param_get_planar_lidar_prefix(BotParam *param, const char *plidar_name, char *result, int result_size);

  /*
   * Search through all the sensors to find the one which is published on this LCM channel
   */
  char * bot_param_get_sensor_name_from_lcm_channel(BotParam *param, const char * sensor_prefix, const char *channel);
  char * bot_param_get_camera_name_from_lcm_channel(BotParam *param, const char *channel);
  char * bot_param_get_planar_lidar_name_from_lcm_channel(BotParam *param, const char *channel);

  /*
   * get the name of the coordinate frame for this sensor
   */
  char * bot_param_get_sensor_coord_frame(BotParam *param, const char * sensor_prefix, const char *sensor_name);
  char * bot_param_get_camera_coord_frame(BotParam *param, const char *camera_name);
  char * bot_param_get_planar_lidar_coord_frame(BotParam *param, const char *lidar_name);

  /*
   * get the lcm channel for the sensor
   */
  char * bot_param_get_sensor_lcm_channel(BotParam *bot_param, const char * sensor_prefix, const char *sensor_name);
  char * bot_param_get_camera_lcm_channel(BotParam *bot_param, const char *camera_name);
  char * bot_param_get_planar_lidar_lcm_channel(BotParam *bot_param, const char *lidar_name);

  /*
   * Create a BotCamTrans object from the information in the config file
   */
  BotCamTrans* bot_param_get_new_camtrans(BotParam *param, const char *cam_name);

#ifdef __cplusplus
}
#endif

#endif  /* BOT2_PARAM_BOT_PARAM_PARAM_UTIL_H_ */
