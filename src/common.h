#ifndef COMMON_H
#define COMMON_H 1

#define M_PI acos(-1.0)
#include "gl-matrix.h"

#define GLEW_STATIC 1

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
#include <GL/glew.h>
#include <assert.h>


#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


// Uncomment the following flag to debug OpenGL calls
//#define DEBUG 1

#ifdef DEBUG
#define GL_DEBUG_ASSERT() assert(glGetError() == GL_NO_ERROR)
#else
#define GL_DEBUG_ASSERT() (void)0
#endif



#endif

