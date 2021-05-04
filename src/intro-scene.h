/*
 * William Emerison Six
 *
 * Copyright 2018 - William Emerison Six
 * All rights reserved
 * Distributed under Apache 2.0
 */

#ifndef INTROSCENE_H
#define INTROSCENE_H 1

#include "common.h"

BEGIN_C_DECLS

extern struct scene_callbacks intro_scene_callbacks;

void intro_scene_init_scene();

void intro_scene_draw_scene();

void intro_scene_update_wall_colors();

void intro_scene_leave_scene();

void intro_scene_window_size_callback(GLFWwindow *window, int width,
                                      int height);

void intro_scene_draw_nuklear(struct nk_context *ctx);

END_C_DECLS
#endif
