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
//#define GL_GLEXT_PROTOTYPES 1 // not sure why I need to do this
#include <GL/glew.h>


#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#endif
