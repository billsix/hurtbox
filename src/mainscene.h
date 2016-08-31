#ifndef MAINSCENE_H
#define MAINSCENE_H 1
/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include "common.h"
#include "main.h"

BEGIN_C_DECLS

void
main_scene_init_scene();

void
main_scene_controller_handle_axis(const SDL_ControllerAxisEvent * const controllerAxisEvent);

void
main_scene_controller_handle_button (const SDL_ControllerButtonEvent * constsdlEvent);

void
main_scene_draw_scene(const Uint8 * const state);

void
main_scene_update_wall_colors();


END_C_DECLS
#endif
