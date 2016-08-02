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
    camera.x -= ( GLdouble ) sin( camera.rotationY) * left_axis.vertical;
    camera.z -= ( GLdouble ) cos( camera.rotationY) * left_axis.vertical;

    camera.x -= ( GLdouble ) cos(camera.rotationY) * left_axis.horizontal;
    camera.z += ( GLdouble ) sin(camera.rotationY) * left_axis.horizontal;

    camera.rotationX += right_axis.vertical;
    camera.rotationY += right_axis.horizontal;

  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLdouble cameraMatrix[16];
  mat4_identity(cameraMatrix);

  // translate from world coordinates to camera coordinates
  {
    mat4_rotateX(cameraMatrix,
                camera.rotationX,
                cameraMatrix);
    mat4_rotateY(cameraMatrix,
                -camera.rotationY,
                cameraMatrix);
    double neg_camera[] = {-camera.x,-camera.y,-camera.z};
    mat4_translate(cameraMatrix,
                   neg_camera,
                   cameraMatrix);
  }

  double current_matrix[16];
  //draw the four walls
  {
    mat4_multiply(projection_matrix,
                  cameraMatrix,
                  current_matrix);

    glLoadMatrixd(current_matrix);

    glColor3fv(wall1Color);
    glBegin(GL_QUADS);
    glVertex3f(-40.0, 0.0, -40.0);
    glVertex3f(40.0, 0.0, -40.0);
    glVertex3f(40.0, 30.0, -40.0);
    glVertex3f(-40.0, 30.0, -40.0);
    glEnd();
  }
  {
    mat4_rotateY(current_matrix, M_PI/2.0, current_matrix);
    glLoadMatrixd(current_matrix);
    glColor3fv(wall2Color);
    glBegin(GL_QUADS);
    glVertex3f(-40.0, 0.0, -40.0);
    glVertex3f(40.0, 0.0, -40.0);
    glVertex3f(40.0, 30.0, -40.0);
    glVertex3f(-40.0, 30.0, -40.0);
    glEnd();
  }
  {
    mat4_rotateY(current_matrix, M_PI/2.0, current_matrix);
    glLoadMatrixd(current_matrix);
    glColor3fv(wall3Color);
    glBegin(GL_QUADS);
    glVertex3f(-40.0, 0.0, -40.0);
    glVertex3f(40.0, 0.0, -40.0);
    glVertex3f(40.0, 30.0, -40.0);
    glVertex3f(-40.0, 30.0, -40.0);
    glEnd();
  }
  {
    mat4_rotateY(current_matrix, M_PI/2.0, current_matrix);
    glLoadMatrixd(current_matrix);
    glColor3fv(wall4Color);
    glBegin(GL_QUADS);
    glVertex3f(-40.0, 0.0, -40.0);
    glVertex3f(40.0, 0.0, -40.0);
    glVertex3f(40.0, 30.0, -40.0);
    glVertex3f(-40.0, 30.0, -40.0);
    glEnd();
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
      left_axis.horizontal = -((double)value / (double)range);
    }
  }
  else if(axis == 1){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      left_axis.vertical = 0.0;
    }
    else{
      left_axis.vertical = -((double)value / (double)range) ;
    }
  }
  else if(axis == 2){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      right_axis.horizontal = 0.0;
    }
    else{
      right_axis.horizontal = -((double)value / (double)range) * 0.1;
    }
  }
  else if(axis == 3){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      right_axis.vertical = 0.0;
    }
    else{
      right_axis.vertical = -((double)value / (double)range) * 0.1;
    }
  }
}

void
main_scene_handle_key(SDL_Keycode *sym){
  switch(*sym){
  case SDLK_UP:
    camera.x -= ( GLdouble ) sin( camera.rotationY);
    camera.z -= ( GLdouble ) cos( camera.rotationY);
    break;
  case SDLK_DOWN:
    camera.x += ( GLdouble ) sin( camera.rotationY);
    camera.z += ( GLdouble ) cos( camera.rotationY);
    break;
  case SDLK_LEFT:
    camera.rotationY += ( GLdouble )0.1;
    break;
  case SDLK_RIGHT:
    camera.rotationY -= ( GLdouble )0.1;
    break;
  default:
    break;
  }
}
