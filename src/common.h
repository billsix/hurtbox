#ifndef COMMON_H
#define COMMON_H 1

#define M_PI acos(-1.0)
#include "gl-matrix.h"

#ifdef __cplusplus
#define BEGIN_C_DECLS extern "C" {
#define END_C_DECLS }
#else
#define BEGIN_C_DECLS
#define END_C_DECLS
#endif

#if HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef _WINDOWS
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#endif
#include "SDL.h"
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#define GL_GLEXT_PROTOTYPES 1 // not sure why I need to do this
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glcorearb.h>
#endif

#endif
