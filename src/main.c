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
#ifdef WIN32
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
#include "hb-imgui.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_GLContext glcontext;

SDL_bool quitMainLoop = SDL_FALSE;
int returnCode = 0;

struct camera camera = { .x = 0.0,
                         .y = 5.0,
                         .z = 0.0,
                         .rotationX = 0.0,
                         .rotationY = 0.0};

int main(int argc, char** argv)
{
  if(initSDL()){
    returnCode = 1;
    goto cleanSDL;
  }
  initGL();
  imgui_init();

  // Main loop
  SDL_bool done = SDL_FALSE;
  while (!done)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
        {
          imgui_process_events(&event);
          if (event.type == SDL_QUIT){
            done = SDL_TRUE;
          }
          if(imgui_wants_event()){
            continue;
          }
          switch(event.type){
          case SDL_MOUSEBUTTONDOWN:
            break;
          case SDL_KEYDOWN:
            handleKey(&event.key.keysym.sym);
            break;
          case SDL_WINDOWEVENT:
            handleWindowEvent(&event);
            break;
          }
        }
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      drawScene();
      drawIMGUI();
      SDL_GL_SwapWindow(window);
      //SDL_Delay(5000);
    }

  // Cleanup
  imgui_shutdown();
 cleanSDL:
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return returnCode;
}

// TODO -- this is really a thing?
#ifdef WIN32
int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	main(0, NULL);
}
#endif

int initSDL(){
  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
      printf("Error: %s\n", SDL_GetError());
      return -1;
    }
  // Setup window
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);

  // TODO -- error out if creation fails
  window = SDL_CreateWindow("HurtBox",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            1280,
                            720,
                            SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  glcontext = SDL_GL_CreateContext(window);
  return 0;
}


void initGL()
{
  glClearColor(0,0,0,1);
  glShadeModel( GL_SMOOTH );
  glClearDepth( 1.0f );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  {
    int w, h;
    SDL_GetWindowSize(window,&w,&h);
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
  }
}


void drawScene()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glRotatef(camera.rotationX, 1.0f, 0.0f, 0.0f);
  glRotatef(360.0f - camera.rotationY, 0.0f, 1.0f, 0.0f);
  glTranslated(-camera.x, -camera.y, -camera.z);

  {
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUADS);
    glVertex3f(-20.0, 0.0, -20.0);
    glVertex3f(20.0, 0.0, -20.0);
    glVertex3f(20.0, 10.0, -20.0);
    glVertex3f(-20.0, 10.0, -20.0);
    glEnd();
    glPopMatrix();
  }
  {
    glPushMatrix();
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    glColor3f(0.0,1.0,1.0);
    glBegin(GL_QUADS);
    glVertex3f(-20.0, 0.0, -20.0);
    glVertex3f(20.0, 0.0, -20.0);
    glVertex3f(20.0, 10.0, -20.0);
    glVertex3f(-20.0, 10.0, -20.0);
    glEnd();
    glPopMatrix();
  }
  {
    glPushMatrix();
    glRotatef(180, 0.0f, 1.0f, 0.0f);
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_QUADS);
    glVertex3f(-20.0, 0.0, -20.0);
    glVertex3f(20.0, 0.0, -20.0);
    glVertex3f(20.0, 10.0, -20.0);
    glVertex3f(-20.0, 10.0, -20.0);
    glEnd();
    glPopMatrix();
  }
  {
    glPushMatrix();
    glRotatef(270.0, 0.0f, 1.0f, 0.0f);
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_QUADS);
    glVertex3f(-20.0, 0.0, -20.0);
    glVertex3f(20.0, 0.0, -20.0);
    glVertex3f(20.0, 10.0, -20.0);
    glVertex3f(-20.0, 10.0, -20.0);
    glEnd();
    glPopMatrix();
  }
}

void handleKey(SDL_Keycode *sym){
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

void handleWindowEvent(SDL_Event *event){
  switch (event->window.event){
  case SDL_WINDOWEVENT_RESIZED:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    {
      int w = event->window.data1, h = event->window.data2;
      gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
    }
    break;
  }
}
