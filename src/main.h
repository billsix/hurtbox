#ifndef MAIN_H
#define MAIN_H 1
/*
 * William Emerison Six
 *
 * Copyright 2016-2017 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */


BEGIN_C_DECLS

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_GLContext glcontext;
extern SDL_PixelFormat RGBAFormat;

struct camera{
  GLfloat x, y, z;
  GLfloat rotationX, rotationY;
};



struct scene_callbacks{
  void (*init_scene)();
  void (*leave_scene)();

  void (*draw_scene)(const Uint8 * const state);
  void (*handle_window_event)(const SDL_Event* const event);
  void (*draw_nuklear)(struct nk_context *ctx);

  void(*handle_controller_button_event) (const SDL_ControllerButtonEvent * const e);
  void(*handle_controller_axis_motion) (const SDL_ControllerAxisEvent * const e);
};

extern struct camera camera;

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
