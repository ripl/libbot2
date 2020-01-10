/*
 * This file is part of bot2-vis.
 *
 * bot2-vis is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-vis is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-vis. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_VIS_BOT_VIS_RWX_H_
#define BOT2_VIS_BOT_VIS_RWX_H_

/**
 * @defgroup BotVisRwx Renderware (.rwx) mesh model rendering
 * @brief Parsing and rendering .rwx models
 * @ingroup BotVisGl
 * @include: bot_vis/bot_vis.h
 *
 * Linking: `pkg-config --libs bot2-vis`
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>

typedef struct _bot_rwx_vertex
{
    double pos[3];
    int id;
} BotRwxVertex;

typedef struct _bot_rwx_triangle
{
    int vertices[3];
} BotRwxTriangle;

typedef struct _bot_rwx_clump
{
    double color[3];
    double diffuse;
    double specular;
    double opacity;
    double ambient;
    char *name;
    BotRwxVertex *vertices;
    BotRwxTriangle *triangles;
    int nvertices;
    int ntriangles;
} BotRwxClump;

typedef struct _bot_rwx_model
{
    GList *clumps;
    int nclumps;
} BotRwxModel;

BotRwxModel * bot_rwx_model_create( const char *fname );

void bot_rwx_model_destroy( BotRwxModel *model );

void bot_rwx_model_apply_transform( BotRwxModel *model, double m[16]);

void bot_rwx_model_gl_draw( BotRwxModel *model );

void bot_rwx_model_get_extrema (BotRwxModel * model,
        double minv[3], double maxv[3]);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  /* BOT2_VIS_BOT_VIS_RWX_H_ */
