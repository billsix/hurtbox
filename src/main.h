/*
 * William Emerison Six
 *
 * Copyright 2016-2017 - William Emerison Six
 * All rights reserved
 * Distributed under Apache 2.0
 */

#ifndef MAIN_H
#define MAIN_H 1


BEGIN_C_DECLS

extern GLFWwindow* window;
extern bool guiEnable;

struct camera{
  GLfloat x, y, z;
  GLfloat rotationX, rotationY;
};



struct scene_callbacks{
  void (*init_scene)();
  void (*leave_scene)();
  void (*draw_scene)();

  void (*draw_nuklear)(struct nk_context *ctx);

};

extern struct camera camera;

void
initGL();

void
drawScene();



END_C_DECLS
#endif
