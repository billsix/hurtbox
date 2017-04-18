/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include <stdio.h>
#include "common.h"
#include "main.h"
#include "gl-matrix.h"
#include "mainscene.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_GLContext glcontext;
SDL_PixelFormat RGBAFormat;



// TODO -- make a data structure to reprsent the full controller,
// along with multiple controllers
struct axis left_axis = {
  .horizontal = 0.0,
  .vertical = 0.0
};

struct axis right_axis = {
  .horizontal = 0.0,
  .vertical = 0.0
};



static void
init_pixel_format()
{

  RGBAFormat.palette = 0;
  RGBAFormat.BitsPerPixel = 32; RGBAFormat.BytesPerPixel = 4;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  RGBAFormat.Rmask = 0xFF000000; RGBAFormat.Rshift =  0; RGBAFormat.Rloss = 0;
  RGBAFormat.Gmask = 0x00FF0000; RGBAFormat.Gshift =  8; RGBAFormat.Gloss = 0;
  RGBAFormat.Bmask = 0x0000FF00; RGBAFormat.Bshift = 16; RGBAFormat.Bloss = 0;
  RGBAFormat.Amask = 0x000000FF; RGBAFormat.Ashift = 24; RGBAFormat.Aloss = 0;
#else
  RGBAFormat.Rmask = 0x000000FF; RGBAFormat.Rshift = 24; RGBAFormat.Rloss = 0;
  RGBAFormat.Gmask = 0x0000FF00; RGBAFormat.Gshift = 16; RGBAFormat.Gloss = 0;
  RGBAFormat.Bmask = 0x00FF0000; RGBAFormat.Bshift =  8; RGBAFormat.Bloss = 0;
  RGBAFormat.Amask = 0xFF000000; RGBAFormat.Ashift =  0; RGBAFormat.Aloss = 0;
#endif

}



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

  init_pixel_format();


  glcontext = SDL_GL_CreateContext(window);
  GL_DEBUG_ASSERT();
  // init GLEW
  glewExperimental = GL_TRUE;
  glewInit();
  // TODO - figure out why this isn't working on Windows
  SDL_GL_MakeCurrent(window,glcontext);
  // log opengl version
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                 SDL_LOG_PRIORITY_INFO,
                 "OpenGL version loaded: %s\n",
                 glGetString(GL_VERSION));

  // enable vsync so that the framerate doesn't go crazy, thus
  // messing up inputs and movement rates
  if(-1 == SDL_GL_SetSwapInterval(1))
    {
      SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                     SDL_LOG_PRIORITY_INFO,
                     "VSync not set correctly\n");

    }
  // initialize OpenGL
  {
    glClearColor(0,0,0,1);
    GL_DEBUG_ASSERT();
    glClearDepth( 1.0f );
    GL_DEBUG_ASSERT();
    glEnable( GL_DEPTH_TEST );
    GL_DEBUG_ASSERT();
    glDepthFunc( GL_LEQUAL );
    GL_DEBUG_ASSERT();
  }

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


  // use the main scene's callbacks
  struct scene_callbacks current_scene = main_scene_callbacks;
  (*current_scene.init_scene)();

  // The event loop.  Keep on truckin'.
  SDL_bool done = SDL_FALSE;
  while (!done)
    {
      // put a SDL Event on the stack so that SDL can be polled
      SDL_Event event;
      while (SDL_PollEvent(&event))
        {
          // to quote the illustrious Arnetta the Mood Setta, "I quit this bitch"
          if (event.type == SDL_QUIT){
            done = SDL_TRUE;
          }

          switch(event.type)
            {
            case SDL_CONTROLLERBUTTONDOWN:
              {
                (*current_scene.handle_controller_button_event)(&event.cbutton);
                break;
              }
            case SDL_CONTROLLERBUTTONUP:
              {
                (*current_scene.handle_controller_button_event)(&event.cbutton);
                break;
              }

            case SDL_CONTROLLERAXISMOTION:
              {
                (*current_scene.handle_controller_axis_motion)(&event.caxis);
                break;
              }
            }
          switch(event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
              break;
            case SDL_WINDOWEVENT:
              (*current_scene.handle_window_event)(&event);
              break;
            }
        }

      const Uint8 *state = SDL_GetKeyboardState(NULL);
      (*current_scene.draw_scene)(state);
      SDL_GL_SwapWindow(window);
    }

  // Cleanup
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
