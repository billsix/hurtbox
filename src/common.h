#ifndef COMMON_H
#define COMMON_H 1

#define M_PI acos(-1.0)
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

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

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
#define GL_DEBUG_ASSERT() { \
  GLenum error = glGetError(); \
  if (error != GL_NO_ERROR) \
    {                                                                   \
      switch(error){                                                    \
      case GL_INVALID_ENUM:                                             \
        printf("An unacceptable value is specified for an enumerated "  \
               "argument. The offending command is ignored and has no " \
               "other side effect than to set the error flag.");        \
        break;                                                          \
      case GL_INVALID_VALUE:                                            \
        printf("A numeric argument is out of range. The offending "     \
               "command is ignored and has no other side effect than "  \
               "to set the error flag.");                               \
        break;                                                          \
      case GL_INVALID_OPERATION:                                        \
        printf("The specified operation is not allowed in the current " \
               "state. The offending command is ignored and has no "    \
               "other side effect than to set the error flag.");        \
        break;                                                          \
      case GL_INVALID_FRAMEBUFFER_OPERATION:                            \
        printf("The framebuffer object is not complete. The offending " \
               "command is ignored and has no other side effect than "  \
               "to set the error flag.");                               \
        break;                                                          \
      case GL_OUT_OF_MEMORY:                                            \
        printf("There is not enough memory left to execute the "        \
               "command. The state of the GL is undefined, except for " \
               "the state of the error flags, after this error is "     \
               "recorded."); \
        break;                                                          \
      case GL_STACK_UNDERFLOW:                                          \
        printf("An attempt has been made to perform an operation that " \
               "would cause an internal stack to underflow.");          \
        break;                                                          \
      case GL_STACK_OVERFLOW:                                           \
        printf("An attempt has been made to perform an operation that " \
               "would cause an internal stack to overflow. ");          \
        break;                                                          \
      }                                                                 \
      assert (error != GL_NO_ERROR);                                    \
    }                                                                   \
  }
#else
#define GL_DEBUG_ASSERT() NOOP()
#endif

#define BUFFER_OFFSET(offset) ((void *) (offset))

#endif

