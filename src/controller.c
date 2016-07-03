/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */


#if HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef _WINDOWS
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#endif
#include "SDL.h"
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <stdio.h>
#include "main.h"


struct axis{
  float horizontal;
  float vertical;
};

static
struct axis left_axis = { .horizontal = 0.0,
                          .vertical = 0.0};

static
struct axis right_axis = { .horizontal = 0.0,
                           .vertical = 0.0};



void controller_handle_axis(SDL_ControllerAxisEvent controllerAxisEvent){
  Uint32 timestamp	= controllerAxisEvent.timestamp;
  SDL_JoystickID which = controllerAxisEvent.which;
  Uint8 axis = controllerAxisEvent.axis;
  Sint16 value = controllerAxisEvent.value; //the axis value (range: -32768 to 32767)

  if(axis == 0){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      left_axis.horizontal = 0.0;
    }
    else{
      left_axis.horizontal = -((float)value / (float)range) * 2.0;
    }
  }
  else if(axis == 1){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      left_axis.vertical = 0.0;
    }
    else{
      left_axis.vertical = -((float)value / (float)range) * 2.0;
    }
  }
  else if(axis == 2){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      right_axis.horizontal = 0.0;
    }
    else{
      right_axis.horizontal = -((float)value / (float)range) * 2.0;
    }
  }
  else if(axis == 3){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      right_axis.vertical = 0.0;
    }
    else{
      right_axis.vertical = -((float)value / (float)range) * 2.0;
    }
  }
}



void controller_update_camera()
{
  camera.x -= ( GLfloat ) sin( camera.rotationY * DEGREES_TO_RADIANS ) * left_axis.vertical;
  camera.z -= ( GLfloat ) cos( camera.rotationY * DEGREES_TO_RADIANS ) * left_axis.vertical;

  camera.x -= ( GLfloat ) cos(camera.rotationY * DEGREES_TO_RADIANS ) * left_axis.horizontal;
  camera.z += ( GLfloat ) sin(camera.rotationY * DEGREES_TO_RADIANS) * left_axis.horizontal;

  camera.rotationX += right_axis.vertical;
  camera.rotationY += right_axis.horizontal;


}
