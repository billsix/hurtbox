#ifndef MAIN_H
#define MAIN_H 1
/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include "common.h"

BEGIN_C_DECLS

#define DEGREES_TO_RADIANS  0.0174532925f
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_GLContext glcontext;
struct camera{
  GLfloat x, y, z;
  GLfloat rotationX, rotationY;
};
extern struct camera camera;


// test data
extern float wall1Color[3];
extern float wall2Color[3];
extern float wall3Color[3];
extern float wall4Color[3];

void
initGL();

int
initSDL();

void
drawScene();

void
handleKey(SDL_Keycode *sym);

void
handleWindowEvent(SDL_Event *event);

END_C_DECLS
#endif
