/*
 * William Emerison Six
 *
 * Copyright 2016-2018 - William Emerison Six
 * All rights reserved
 * Distributed under Apache 2.0
 */

#include "common.h"

int number_of_controllers = 0;

void init_controllers() {
  int number_of_controllers = glfwJoystickPresent(GLFW_JOYSTICK_1);
  printf("number of controllers %d\n", number_of_controllers);
}
