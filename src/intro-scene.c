/*
* William Emerison Six
*
* Copyright 2017 - William Emerison Six
* All rights reserved
* Distributed under LGPL 2.1 or Apache 2.0
*/

#include "common.h"
#include "main.h"

#include "shader.h"
#include "intro-scene.h"
#include "gl-matrix.h"


extern struct scene_callbacks
intro_scene_callbacks;

void
intro_scene_init_scene(){

}

void
intro_scene_controller_handle_axis(const SDL_ControllerAxisEvent * const controllerAxisEvent) {

}

void
intro_scene_controller_handle_button(const SDL_ControllerButtonEvent * const sdlEvent) {

}

void
intro_scene_draw_scene(const Uint8 * const state) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_DEBUG_ASSERT();
}

void
intro_scene_update_wall_colors() {

}

void
intro_scene_handle_window_event(const SDL_Event* const event) {

}

void
intro_scene_leave_scene() {

}

void
intro_scene_draw_nuklear(struct nk_context *ctx) {

}


