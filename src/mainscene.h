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
main_scene_controller_handle_axis(SDL_ControllerAxisEvent controllerAxisEvent);

void
main_scene_controller_handle_button (SDL_ControllerButtonEvent sdlEvent);

void
main_scene_handle_key(SDL_Keycode *sym);

void
main_scene_draw_scene();


END_C_DECLS
#endif
