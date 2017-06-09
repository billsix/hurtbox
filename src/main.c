/*
 * William Emerison Six
 *
 * Copyright 2016-2017 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include <stdio.h>
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#include "common.h"
#include "main.h"
#include "gl-matrix.h"
#include "mainscene.h"
#include "intro-scene.h"




SDL_Window *window;
SDL_Renderer *renderer;
SDL_GLContext glcontext;
SDL_PixelFormat RGBAFormat;








#ifdef __cplusplus
extern "C"
#endif
int
main(int argc, char** argv)
{
  // init SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_ERROR,
                   "Error: Could not init SDL video %s\n",
                   SDL_GetError());
    return 1;
  }
  if (SDL_Init(SDL_INIT_AUDIO) != 0){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "Could not init SDL audio");
  }
  if (SDL_Init(SDL_INIT_TIMER) != 0){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "Could not init SDL timer");
  }
  if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "Could not init SDL game controller");
  }
  if (SDL_Init(SDL_INIT_HAPTIC)){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "Could not init SDL haptic feedback");
  }

  // init SDL_image
  IMG_Init(IMG_INIT_PNG);

  // Initialize SDL Attributes
  {
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
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
                                        SDL_WINDOW_OPENGL
					|SDL_WINDOW_RESIZABLE
                                        |SDL_WINDOW_ALLOW_HIGHDPI))){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		   SDL_LOG_PRIORITY_ERROR,
		   "Could not create window: %s\n",
		   SDL_GetError());
    return 1;
  }

  // init pixel format
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

  glcontext = SDL_GL_CreateContext(window);

  if (0 != SDL_GL_MakeCurrent(window, glcontext)) {
    // Unrecoverable error, exit here.
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "SDL_Init failed: %s\n", SDL_GetError());
    return 1;
  }

  GL_DEBUG_ASSERT();
  // init GLEW
  glewExperimental = GL_TRUE;
  {
    GLenum err = glewInit();
    if (GLEW_OK != err)
      {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                       SDL_LOG_PRIORITY_ERROR,
                       "Could not init glew\n");
      }
  }

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
    GL_DEBUG_ASSERT();
    glClearColor(0.0,0.0,0.0,1.0);
    GL_DEBUG_ASSERT();
    glClearDepth( 1.0f );
    GL_DEBUG_ASSERT();
    glEnable( GL_DEPTH_TEST );
    GL_DEBUG_ASSERT();
    glDepthFunc( GL_LEQUAL );
    GL_DEBUG_ASSERT();
    glBlendEquation(GL_FUNC_ADD);
    GL_DEBUG_ASSERT();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_DEBUG_ASSERT();

  }

  init_controllers();

  {
    int32_t w, h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0,
               w, h);
  }


  struct scene_callbacks intro_scene_callbacks = {
    .init_scene = intro_scene_init_scene,
    .leave_scene = intro_scene_leave_scene,

    .draw_scene = intro_scene_draw_scene,
    .handle_window_event = intro_scene_handle_window_event,
    .draw_nuklear = intro_scene_draw_nuklear,

    .handle_controller_button_event = intro_scene_controller_handle_button,
    .handle_controller_axis_motion = intro_scene_controller_handle_axis
  };
  // use the intro scene's callbacks
  struct scene_callbacks current_scene = intro_scene_callbacks;
  // init the intro scene
  (*current_scene.init_scene)();


  struct scene_callbacks main_scene_callbacks = {
    .init_scene = main_scene_init_scene,
    .leave_scene = main_scene_leave_scene,

    .draw_scene = main_scene_draw_scene,
    .handle_window_event = main_scene_handle_window_event,
    .draw_nuklear = main_scene_draw_nuklear,

    .handle_controller_button_event = main_scene_controller_handle_button,
    .handle_controller_axis_motion = main_scene_controller_handle_axis
  };
  // init the main scene
  (*main_scene_callbacks.init_scene)();




  // nuklear context
  struct nk_context *ctx = nk_sdl_init(window);
  {
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {
      struct nk_font_atlas *atlas;
      nk_sdl_font_stash_begin(&atlas);
      /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
      /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16, 0);*/
      /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
      /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
      /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
      /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
      nk_sdl_font_stash_end();
      /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
      /*nk_style_set_font(ctx, &roboto->handle)*/;}

    /* style.c */
    /*set_style(ctx, THEME_WHITE);*/
    /*set_style(ctx, THEME_RED);*/
    /*set_style(ctx, THEME_BLUE);*/
    /*set_style(ctx, THEME_DARK);*/

  }

  // The event loop.  Keep on truckin'.
  SDL_bool done = SDL_FALSE;
  while (!done)
    {
      // put a SDL Event on the stack so that SDL can be polled
      SDL_Event event;
      nk_input_begin(ctx);
      while (SDL_PollEvent(&event))
        {
          // to quote the illustrious Arnetta the Mood Setta, "I quit this bitch"
          if (event.type == SDL_QUIT){
            done = SDL_TRUE;
          }

	  // need to only handle events that nuklear wants, I had the same issue with IMGUI at first,
	  // I know they have some way to handle this
	  nk_sdl_handle_event(&event);

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
              {
                int32_t w, h;
                SDL_GetWindowSize(window,&w,&h);
                glViewport(0, 0,
                           w, h);
                (*current_scene.handle_window_event)(&event);
                break;
              }
            }
        }
      nk_input_end(ctx);

      const Uint8 *state = SDL_GetKeyboardState(NULL);
      (*current_scene.draw_scene)(state);

      // nuklear
      {
	/* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
	 * with blending, scissor, face culling, depth test and viewport and
	 * defaults everything back into a default state.
	 * Make sure to either a.) save and restore or b.) reset your own state after
	 * rendering the UI. */
	// Backup GL state
	GLint last_program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	GLint last_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_active_texture;
        glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
	GLint last_array_buffer;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	GLint last_element_array_buffer;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
	GLint last_vertex_array;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
	GLint last_blend_src;
        glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
	GLint last_blend_dst;
        glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
	GLint last_blend_equation_rgb;
        glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
	GLint last_blend_equation_alpha;
        glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
	GLint last_viewport[4];
        glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4];
        glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
	GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
	GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
	GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);


	// draw the scene specific controls
	(*current_scene.draw_nuklear)(ctx);


	//draw the scene manager UI
	{
          if (nk_begin(ctx,
                       "Override Scene",
                       nk_rect(0, 0, 200, 200),
                       NK_WINDOW_BORDER
                       | NK_WINDOW_MOVABLE
                       | NK_WINDOW_SCALABLE
                       | NK_WINDOW_MINIMIZABLE
                       | NK_WINDOW_TITLE))
            {

              enum { INTRO, MAIN };
              static int currentSceneRadioButton = INTRO;
              static int property = 20;
              nk_layout_row_dynamic(ctx, 30, 2);

              int8_t oldScene = currentSceneRadioButton;
              if (nk_option_label(ctx, "Intro", currentSceneRadioButton == INTRO)) {
                if (oldScene != INTRO) {
                  (*current_scene.leave_scene)();
                }
                currentSceneRadioButton = INTRO;
                current_scene = intro_scene_callbacks;
              }
              if (nk_option_label(ctx, "Main", currentSceneRadioButton == MAIN)) {
                if (oldScene != MAIN) {
                  (*current_scene.leave_scene)();
                }
                currentSceneRadioButton = MAIN;
                current_scene = main_scene_callbacks;
              }
            }
          nk_end(ctx);
	}

	// render nuklear
	nk_sdl_render(NK_ANTI_ALIASING_ON,
                      MAX_VERTEX_MEMORY,
                      MAX_ELEMENT_MEMORY);

	// Restore modified GL state
	glUseProgram(last_program);
	glActiveTexture(last_active_texture);
	glBindTexture(GL_TEXTURE_2D,
                      last_texture);
	glBindVertexArray(last_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER,
                     last_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                     last_element_array_buffer);
	glBlendEquationSeparate(last_blend_equation_rgb,
                                last_blend_equation_alpha);
	glBlendFunc(last_blend_src,
                    last_blend_dst);
	if (last_enable_blend)
          glEnable(GL_BLEND);
        else
          glDisable(GL_BLEND);
	if (last_enable_cull_face)
          glEnable(GL_CULL_FACE);
        else
          glDisable(GL_CULL_FACE);
	if (last_enable_depth_test)
          glEnable(GL_DEPTH_TEST);
        else
          glDisable(GL_DEPTH_TEST);
	if (last_enable_scissor_test)
          glEnable(GL_SCISSOR_TEST);
        else
          glDisable(GL_SCISSOR_TEST);
	glViewport(last_viewport[0],
                   last_viewport[1],
                   (GLsizei)last_viewport[2],
                   (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0],
                  last_scissor_box[1],
                  (GLsizei)last_scissor_box[2],
                  (GLsizei)last_scissor_box[3]);
      }
      SDL_GL_SwapWindow(window);
    }

  // Cleanup
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
  return 0;
}

