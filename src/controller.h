#ifndef CONTROLLER_H
#define CONTROLLER_H 1
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
controller_handle_axis(SDL_ControllerAxisEvent controllerAxisEvent);

void
controller_update_camera();
END_C_DECLS


#endif
