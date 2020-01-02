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

#ifndef BOT2_CORE_BOT_CORE_PPM_H_
#define BOT2_CORE_BOT_CORE_PPM_H_

#include <stdint.h>
#include <stdio.h>

/**
 * @defgroup BotCorePPM PPM/PGM
 * @brief Reading and writing PPM/PGM files
 * @ingroup BotCoreIO
 * @include: bot_core/bot_core.h
 *
 * TODO
 *
 * Linking: `pkg-config --libs bot2-core`
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

int bot_ppm_read (FILE *fp, uint8_t **pixels,
        int *width, int *height, int *rowstride);

int bot_ppm_read_fname(const char* fname, uint8_t** pixels,
        int* width, int* height, int* rowstride);

int bot_ppm_write (FILE *fp, const uint8_t *pixels,
        int width, int height, int rowstride);

int bot_ppm_write_fname(const char* fname, const uint8_t* pixels,
        int width, int height, int rowstride);

int bot_ppm_write_bottom_up (FILE *fp, uint8_t *pixels,
        int width, int height, int rowstride);

int bot_pgm_read (FILE *fp, uint8_t **pixels,
        int *width, int *height, int *rowstride);

int bot_pgm_read_fname(const char *fname, uint8_t **pixels,
        int *width, int *height, int *rowstrde);

int bot_pgm_write (FILE *fp, const uint8_t *pixels,
        int width, int height, int rowstride);

int bot_pgm_write_fname(const char *fname, const uint8_t * pixels,
        int width, int height, int rowstrde);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  /* BOT2_CORE_BOT_CORE_PPM_H_ */
