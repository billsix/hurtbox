#ifndef HB_IMGUI_H
#define HB_IMGUI_H 1
/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include "common.h"

BEGIN_C_DECLS

void
imgui_init();

void
imgui_process_events(SDL_Event *event);

SDL_bool
imgui_wants_event();

void
imgui_draw();

void
imgui_shutdown();

END_C_DECLS

#endif
