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

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_GLContext glcontext;
struct camera{
  GLfloat x, y, z;
  GLfloat rotationX, rotationY;
};


struct axis{
  float horizontal;
  float vertical;
};

struct scene_callbacks{
  void (*init_scene)();
  void (*handle_controller_button_event) (const SDL_ControllerButtonEvent * const e);
  void (*handle_controller_axis_motion) (const SDL_ControllerAxisEvent * const e);
  void (*draw_scene)(const Uint8 * const state);
  void (*handle_window_event)(const SDL_Event* const event);


};

extern struct camera camera;
extern struct axis left_axis;
extern struct axis right_axis;

// test data
extern float wallColors[];

void
initGL();

int
initSDL();

void
drawScene();

void
handleKey(SDL_Keycode *sym);

END_C_DECLS
#endif
