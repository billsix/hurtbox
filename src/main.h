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
  GLdouble x, y, z;
  GLdouble rotationX, rotationY;
};

extern GLdouble projection_matrix[16];
extern GLdouble modelview_matrix[16];


struct axis{
  double horizontal;
  double vertical;
};

struct scene_callbacks{
  void (*init_scene)();
  void (*handle_controller_button_event) (SDL_ControllerButtonEvent e);
  void (*handle_controller_axis_motion) (SDL_ControllerAxisEvent e);
  void (*handle_key)(SDL_Keycode *sym);
  void (*draw_scene)();

};

extern struct camera camera;
extern struct axis left_axis;
extern struct axis right_axis;

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
