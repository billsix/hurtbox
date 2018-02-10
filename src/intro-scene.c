/*
* William Emerison Six
*
* Copyright 2018 - William Emerison Six
* All rights reserved
* Distributed under Apache 2.0
*/

#include "common.h"
#include "main.h"

#include "shader.h"
#include "intro-scene.h"
#include "gl-matrix-stack.h"


extern struct scene_callbacks
intro_scene_callbacks;

void
intro_scene_init_scene(){

}

void
intro_scene_draw_scene(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_DEBUG_ASSERT();

}

void
intro_scene_update_wall_colors() {

}


void
intro_scene_leave_scene() {

}

void
intro_scene_window_size_callback(GLFWwindow* window, int width, int height)
{

}


void
intro_scene_draw_nuklear(struct nk_context *ctx) {

}
