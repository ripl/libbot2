// -*- mode: c -*-
// vim: set filetype=c :

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

#ifndef BOT2_VIS_BOT_VIS_GLMINT_H_
#define BOT2_VIS_BOT_VIS_GLMINT_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern GLenum _glmTextureTarget;

// private routines from glm_util.c
extern char* __glmStrStrip(const char* string);
#ifdef HAVE_STRDUP
#define __glmStrdup strdup
#else
extern char* __glmStrdup(const char* string);
#endif
extern void __glmWarning(char* format, ...);
extern void __glmFatalError(char* format, ...);
extern void __glmFatalUsage(char* format, ...);
extern char* __glmDirName(char* path);
void __glmReportErrors(void);

#ifdef DEBUG
#define DBG_(_x) ((void)(_x))
#else
#define DBG_(_x) ((void)0)
#endif

#ifdef GLDEBUG
#define GLDBG_(_x)                                                            \
  {                                                                           \
    GLenum ret = (_x);                                                        \
    if (ret != GL_NO_ERROR)                                                   \
      __glmWarning("OpenGL error at %d : %s", __LINE__, gluErrorString(ret)); \
  }
#else
#define GLDBG_(_x) ((void)0)
#endif

#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif

#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif

GLubyte* glmReadDevIL(const char*, GLboolean, int*, int*, int*);
GLubyte* glmReadJPG(const char*, GLboolean, int*, int*, int*);
GLubyte* glmReadPNG(const char*, GLboolean, int*, int*, int*);
GLubyte* glmReadSDL(const char*, GLboolean, int*, int*, int*);
GLubyte* glmReadSimage(const char*, GLboolean, int*, int*, int*);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_VIS_BOT_VIS_GLMINT_H_
