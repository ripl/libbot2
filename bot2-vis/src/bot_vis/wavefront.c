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

/*
 * wavefront.c
 *
 * Public API for rendering 3D models that are represented in the
 * Wavefront OBJ (geometry) and MTL (material properties) file formats.
 *
 * The API is largely a wrapper for the GLM library
 *
 * Created on: Dec 2, 2010
 *     Author: mwalter
 *
 */

#include "wavefront.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "glm.h"

struct _BotWavefrontModel
{
    GLMmodel *glm_model;
};

BotWavefrontModel *
bot_wavefront_model_create (const char *filename)
{
    BotWavefrontModel *model = calloc (1, sizeof (BotWavefrontModel));

    model->glm_model = glmReadOBJ (filename);

    if (model->glm_model == NULL) {
        free (model);
        return NULL;
    }

    return model;
}

void
bot_wavefront_model_destroy (BotWavefrontModel *model)
{
    if (model->glm_model)
        glmDelete (model->glm_model);

    free (model);
}

void
bot_wavefront_model_gl_draw (BotWavefrontModel *model)
{
    // Generate facet normals if there aren't any in the .obj
    if (model->glm_model->numfacetnorms == 0) {
        glmFacetNormals (model->glm_model);
        fprintf (stdout, "Adding %d FacetNormals\n", model->glm_model->numfacetnorms);
    }

    // Generate normals for each vertex if they aren't specified in the .obj file
    // Per the suggestion in glm.c, use 90 deg as the facet outlier threshold
    if (model->glm_model->numnormals == 0) {
        glmVertexNormals (model->glm_model, 90.0, 0);
        fprintf (stdout, "Adding %d VertexNormals\n", model->glm_model->numnormals);
    }

    // Call GLM to draw the object.
    //glDisable(GL_LIGHTING);
    glmDraw (model->glm_model, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    //glEnable(GL_LIGHTING);
}

void
bot_wavefront_model_get_extrema (BotWavefrontModel *model,
                                 double minv[3], double maxv[3])
{
    int i;
    for (i = 0; i < 3; i++) {
        minv[i] = INFINITY;
        maxv[i] = -INFINITY;
    }

    for (i = 1; i <= model->glm_model->numvertices; i++) {
        if (maxv[0] < model->glm_model->vertices[3 * i])
            maxv[0] = model->glm_model->vertices[3 * i];
        if (minv[0] > model->glm_model->vertices[3 * i])
            minv[0] = model->glm_model->vertices[3 * i];

        if (maxv[1] < model->glm_model->vertices[3 * i + 1])
            maxv[1] = model->glm_model->vertices[3 * i + 1];
        if (minv[1] > model->glm_model->vertices[3 * i + 1])
            minv[1] = model->glm_model->vertices[3 * i + 1];

        if (maxv[2] < model->glm_model->vertices[3 * i + 2])
            maxv[2] = model->glm_model->vertices[3 * i + 2];
        if (minv[2] > model->glm_model->vertices[3 * i + 2])
            minv[2] = model->glm_model->vertices[3 * i + 2];
    }

    return;
}
