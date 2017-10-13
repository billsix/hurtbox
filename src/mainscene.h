#ifndef MAINSCENE_H
#define MAINSCENE_H 1
/*
 * William Emerison Six
 *
 * Copyright 2016-2017 - William Emerison Six
 * All rights reserved
 * Distributed under Apache 2.0
 */

#include "common.h"

BEGIN_C_DECLS

extern struct scene_callbacks
main_scene_callbacks;

void
main_scene_init_scene();

void
main_scene_draw_scene();

void
main_scene_update_wall_colors();

void
main_scene_leave_scene();

void
main_scene_draw_nuklear(struct nk_context *ctx);


END_C_DECLS
#endif
