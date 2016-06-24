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
 // TODO -- fix this -- this is dumb to include SDL differently
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
// temporary to test opengl with imgui
double rotation = 0.0;


void drawScene()
{

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
}


int main(int argc, char** argv)
{
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

  imgui_init();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

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
          if (event.type == SDL_KEYDOWN){
          }
          if (event.type == SDL_MOUSEBUTTONDOWN){
            rotation += 10;
            if(rotation > 360.0)
              rotation -= 360.0;
          }

        }

      glClearColor(0,0,0,1);
      glClear(GL_COLOR_BUFFER_BIT);
	  drawScene();
	  drawIMGUI();
      SDL_GL_SwapWindow(window);
	  //SDL_Delay(5000);
    }

  // Cleanup
  //imgui_shutdown();
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
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
