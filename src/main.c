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
#include "gl-matrix.h"

#include "mainscene.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_GLContext glcontext;


GLfloat projection_matrix[16];
GLfloat modelview_matrix[16];

SDL_bool quitMainLoop = SDL_FALSE;


// TODO -- make a data structure to reprsent the full controller,
// along with multiple controllers
struct axis left_axis = { .horizontal = 0.0,
                          .vertical = 0.0};

struct axis right_axis = { .horizontal = 0.0,
                           .vertical = 0.0};


// function pointers to handle events on a per-scene basis
struct scene_callbacks current_scene = {
  .init_scene = main_scene_init_scene,
  .handle_controller_button_event = &main_scene_controller_handle_button,
  .handle_controller_axis_motion = &main_scene_controller_handle_axis,
  .draw_scene = &main_scene_draw_scene
};


int
main(int argc, char** argv)
{
  // TODO - test initilizing individual items, and set flags
  // based off of what is available.
  // for instance, on my Funtoo system, HAPTIC is not enabled,
  // which should not cause hurtbox to quit.
  // Setup SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
      SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		     SDL_LOG_PRIORITY_ERROR,
		     "Error: %s\n",
		     SDL_GetError());
      return 1;
    }

  // Initialize SDL Attributes
  {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
  }

  if(NULL == (window = SDL_CreateWindow("HurtBox",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        1280,
                                        720,
                                        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE))){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		   SDL_LOG_PRIORITY_ERROR,
		   "Could not create window: %s\n",
		   SDL_GetError());
    return 1;
  }

  glcontext = SDL_GL_CreateContext(window);



  GL_DEBUG_ASSERT();

  glewExperimental = GL_TRUE;
  glewInit();


  SDL_GL_MakeCurrent(glcontext, window);

  // log opengl version
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                 SDL_LOG_PRIORITY_INFO,
                 "OpenGL version loaded: %s\n",
                 glGetString(GL_VERSION));


  // initialize OpenGL
  {

    mat4_identity(projection_matrix);
    mat4_identity(modelview_matrix);

    glClearColor(0,0,0,1);
    GL_DEBUG_ASSERT();
    glClearDepth( 1.0f );
    GL_DEBUG_ASSERT();
    glEnable( GL_DEPTH_TEST );
    GL_DEBUG_ASSERT();
    glDepthFunc( GL_LEQUAL );
    GL_DEBUG_ASSERT();

    {
      int w, h;
      SDL_GetWindowSize(window,&w,&h);
      mat4_perspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f,projection_matrix);
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
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		   SDL_LOG_PRIORITY_INFO,
		   "%d\n",
		   SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt"));


    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		   SDL_LOG_PRIORITY_INFO,
		   "%d controllers\n",
		   SDL_NumJoysticks());
    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
      if (SDL_IsGameController(i)) {
        controller = SDL_GameControllerOpen(i);
        if (controller) {
          SDL_Joystick *joy = SDL_GameControllerGetJoystick( controller );
          int instanceID = SDL_JoystickInstanceID( joy );
          break;
        } else {
	  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
			 SDL_LOG_PRIORITY_INFO,
			 "Could not open gamecontroller %i: %s\n",
			 i,
			 SDL_GetError());
        }
      }
    }
  }

  (*current_scene.init_scene)();

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
                (*current_scene.handle_controller_button_event)(event.cbutton);
                break;
              }
            case SDL_CONTROLLERBUTTONUP:
              {
                (*current_scene.handle_controller_button_event)(event.cbutton);
                break;
              }

            case SDL_CONTROLLERAXISMOTION:
              {
                (*current_scene.handle_controller_axis_motion)(event.caxis);
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
            case SDL_WINDOWEVENT:
              handleWindowEvent(&event);
              break;
            }
        }

	  const Uint8 *state = SDL_GetKeyboardState(NULL);
      (*current_scene.draw_scene)(state);
      drawIMGUI();
      SDL_GL_SwapWindow(window);
    }

  // Cleanup
  imgui_shutdown();
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}



void
handleWindowEvent(SDL_Event *event){
  switch (event->window.event){
  case SDL_WINDOWEVENT_RESIZED:
    {
      int w = event->window.data1, h = event->window.data2;
      mat4_perspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f,projection_matrix);
    }
    break;
  }
}


