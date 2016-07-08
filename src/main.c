/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include "common.h"
#include <stdio.h>
#include "main.h"
#include "hb-imgui.h"
#include "controller.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_GLContext glcontext;

SDL_bool quitMainLoop = SDL_FALSE;

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


int main(int argc, char** argv)
{
  int returnCode = 0;
  // TODO - test initilizing individual items, and set flags
  // based off of what is available.
  // for instance, on my Funtoo system, HAPTIC is not enabled,
  // which should not cause hurtbox to quit.
  // Setup SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
      printf("Error: %s\n", SDL_GetError());
      returnCode = 1;
      return returnCode; // sure I could have just returned 1.
    }

  // Initialize SDL Attributes
  {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
  }
  
  if(NULL == (window = SDL_CreateWindow("HurtBox",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        1280,
                                        720,
                                        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE))){
    printf("Could not create window: %s\n", SDL_GetError());
    returnCode = 1;
    return returnCode; // sure I could have just returned 1.
  }

  glcontext = SDL_GL_CreateContext(window);
  /* Note to self.  SDL's docs tell me that
   *   Windows users new to OpenGL should note that, for historical reasons, 
   *   GL functions added after OpenGL version 1.1 are not available by 
   *   default. Those functions must be loaded at run-time, either with 
   *   an OpenGL extension-handling library or with SDL_GL_GetProcAddress() 
   */ 
  
  // initialize OpenGL
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
      gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);
    }
    
  }

  // initialize IMGUI.  Not currently sure what it does.  But I know I need
  // to it
  imgui_init();

  // initialize controllers
  // TODO -- create a data structure to handle the controllers
  {
    // TODO -- Use automake to put this file in the installation directory
    //   and figure out how Visual Studio handles similar things (do they have
    //   a "make install"?)
    printf("%d\n", SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt"));
    
    
    printf("%d controllers\n", SDL_NumJoysticks());
    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
      if (SDL_IsGameController(i)) {
        controller = SDL_GameControllerOpen(i);
        if (controller) {
          SDL_Joystick *joy = SDL_GameControllerGetJoystick( controller );
          int instanceID = SDL_JoystickInstanceID( joy );
          break;
        } else {
          fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
        }
      }
    }
  }
  
  // The event loop.  Keep on truckin'.
SDL_bool done = SDL_FALSE;
    while (!done)
    {
      // put a SDL Event on the stack so that SDL can be polled
      SDL_Event event;
      while (SDL_PollEvent(&event))
        {
          // from the IMGUI demo, I surmise that I need to do this first
          imgui_process_events(&event);

          // to quote the illustrious Arnetta the Mood Setta, "I quit this bitch"
          if (event.type == SDL_QUIT){
            done = SDL_TRUE;
          }

          /* If the user has his mouse over an IMGUI control, the user should
           * expect that the event will be handled by IMGUI.  A few lines down,
           * this code asks IMGUI if it wants the event, and if so it lets IMGUI
           * exclusively handle it.  However, joysticks will not control
           * IMGUI controls, and should be handled firstly.
           */
          
          switch(event.type)
            {
            case SDL_CONTROLLERBUTTONDOWN:
              {
                const SDL_ControllerButtonEvent sdlEvent =  event.cbutton;
                // TODO -- do something
                break;
              }
            case SDL_CONTROLLERBUTTONUP:
              {
                const SDL_ControllerButtonEvent sdlEvent =  event.cbutton;
                // TODO -- do something
                break;
              }
              
            case SDL_CONTROLLERAXISMOTION:
              {
                controller_handle_axis(event.caxis);
                break;
              }
              
            }
          // if IMGUI has focus, let it take the mouse and keyboard event
          if(imgui_wants_event()){
            continue;
          }
          switch(event.type)
            {
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
      controller_update_camera();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      drawScene();
      drawIMGUI();
      SDL_GL_SwapWindow(window);
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

void drawScene()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glRotatef(camera.rotationX, 1.0f, 0.0f, 0.0f);
  glRotatef(360.0f - camera.rotationY, 0.0f, 1.0f, 0.0f);
  glTranslated(-camera.x, -camera.y, -camera.z);

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


