#ifndef COMMON_H
#define COMMON_H 1

#define M_PI acos(-1.0)
#include "gl-matrix.h"
#ifndef _WIN32
#define GLEW_STATIC 1
#endif

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
#include "SDL_image.h"
#include <GL/glew.h>
#include <assert.h>

// nuklear defs
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "../contrib/nuklear/nuklear.h"
#include "../contrib/nuklear/nuklear_sdl_gl3.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define NOOP() (void)0

// Uncomment the following flag to debug OpenGL calls
//#define DEBUG 1

#ifdef DEBUG
#define GL_DEBUG_ASSERT() assert(glGetError() == GL_NO_ERROR)
#else
#define GL_DEBUG_ASSERT() NOOP()
#endif

#define BUFFER_OFFSET(offset) ((void *) (offset))

#endif

