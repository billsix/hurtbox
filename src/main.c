/*
 * William Emerison Six
 *
 * Copyright 2014-2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "SDL2/SDL.h"
#include "main.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>


SDL_bool initSDL()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow("FrameTrap",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH,
                            SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL
                            );

  if (!window) {
    printf("Could not create window: %s\n", SDL_GetError());
    returnCode = 1;
    return SDL_FALSE;
  }

  glcontext = SDL_GL_CreateContext(window);
  if (!glcontext) {
    printf("Could not create context: %s\n", SDL_GetError());
    returnCode = 1;
    return SDL_FALSE;
  }

  renderer = SDL_CreateRenderer(window,
                                -1,
                                (SDL_RENDERER_ACCELERATED
                                 | SDL_RENDERER_PRESENTVSYNC));
  if (!renderer){
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    returnCode = 1;
    return SDL_FALSE;
  }
  return SDL_TRUE;
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);
}

void drawScene()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glRotated(rotation,0,0,1);
  glBegin(GL_QUADS);
  {
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f( 0.5f, -0.5f);
    glVertex2f( 0.5f,  0.5f);
    glVertex2f(-0.5f,  0.5f);
    glEnd();
  }

  glFlush();

}

int main ( void )
{
  if(initSDL() == SDL_FALSE)
    goto cleanup;

  SDL_Event event;
  while(quitMainLoop == SDL_FALSE){
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT){
      quitMainLoop = SDL_TRUE;
    }
    if (event.type == SDL_KEYDOWN){
    }
    if (event.type == SDL_MOUSEBUTTONDOWN){
      rotation += 10;
      if(rotation > 360.0)
        rotation -= 360.0;
    }
    drawScene();
    SDL_GL_SwapWindow(window);
  }

 cleanup:
  SDL_DestroyWindow(window);
  SDL_Quit();
  return returnCode;
}
