/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include "common.h"
#include "main.h"


struct camera camera = { .x = 0.0,
                         .y = 5.0,
                         .z = 0.0,
                         .rotationX = 0.0,
                         .rotationY = 0.0};

// test data
float wall1Color[3] = {1.0,1.0,1.0};
float wall2Color[3] = {0.0,1.0,1.0};
float wall3Color[3] = {0.0,0.0,1.0};
float wall4Color[3] = {1.0,0.0,1.0};


void
main_scene_draw_scene()
{
  // update camera from the controller
  {
    camera.x -= ( GLfloat ) sin( camera.rotationY * DEGREES_TO_RADIANS ) * left_axis.vertical;
    camera.z -= ( GLfloat ) cos( camera.rotationY * DEGREES_TO_RADIANS ) * left_axis.vertical;
    
    camera.x -= ( GLfloat ) cos(camera.rotationY * DEGREES_TO_RADIANS ) * left_axis.horizontal;
    camera.z += ( GLfloat ) sin(camera.rotationY * DEGREES_TO_RADIANS) * left_axis.horizontal;
    
    camera.rotationX += right_axis.vertical;
    camera.rotationY += right_axis.horizontal;
    
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // translate from world coordinates to camera coordinates
  {
    glRotatef(camera.rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(360.0f - camera.rotationY, 0.0f, 1.0f, 0.0f);
    glTranslated(-camera.x, -camera.y, -camera.z);
  }

  // draw the four walls
  {
    glPushMatrix();
    glColor3fv(wall1Color);
    glBegin(GL_QUADS);
    glVertex3f(-40.0, 0.0, -40.0);
    glVertex3f(40.0, 0.0, -40.0);
    glVertex3f(40.0, 30.0, -40.0);
    glVertex3f(-40.0, 30.0, -40.0);
    glEnd();
    glPopMatrix();
  }
  {
    glPushMatrix();
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    glColor3fv(wall2Color);
    glBegin(GL_QUADS);
    glVertex3f(-40.0, 0.0, -40.0);
    glVertex3f(40.0, 0.0, -40.0);
    glVertex3f(40.0, 30.0, -40.0);
    glVertex3f(-40.0, 30.0, -40.0);
    glEnd();
    glPopMatrix();
  }
  {
    glPushMatrix();
    glRotatef(180, 0.0f, 1.0f, 0.0f);
    glColor3fv(wall3Color);
    glBegin(GL_QUADS);
    glVertex3f(-40.0, 0.0, -40.0);
    glVertex3f(40.0, 0.0, -40.0);
    glVertex3f(40.0, 30.0, -40.0);
    glVertex3f(-40.0, 30.0, -40.0);
    glEnd();
    glPopMatrix();
  }
  {
    glPushMatrix();
    glRotatef(270.0, 0.0f, 1.0f, 0.0f);
    glColor3fv(wall4Color);
    glBegin(GL_QUADS);
    glVertex3f(-40.0, 0.0, -40.0);
    glVertex3f(40.0, 0.0, -40.0);
    glVertex3f(40.0, 30.0, -40.0);
    glVertex3f(-40.0, 30.0, -40.0);
    glEnd();
    glPopMatrix();
  }
}


void
main_scene_controller_handle_button (SDL_ControllerButtonEvent sdlEvent)
{

}


void
main_scene_controller_handle_axis(SDL_ControllerAxisEvent controllerAxisEvent){
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

void
main_scene_handle_key(SDL_Keycode *sym){
  switch(*sym){
  case SDLK_UP:
    camera.x -= ( GLfloat ) sin( camera.rotationY * DEGREES_TO_RADIANS );
    camera.z -= ( GLfloat ) cos( camera.rotationY * DEGREES_TO_RADIANS );
    break;
  case SDLK_DOWN:
    camera.x += ( GLfloat ) sin( camera.rotationY * DEGREES_TO_RADIANS );
    camera.z += ( GLfloat ) cos( camera.rotationY * DEGREES_TO_RADIANS );
    break;
  case SDLK_LEFT:
    camera.rotationY += 2.0;
    break;
  case SDLK_RIGHT:
    camera.rotationY -= 2.0;
    break;
  default:
    break;
  }
}
