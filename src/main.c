/*
 * William Emerison Six
 *
 * Copyright 2016-2018 - William Emerison Six
 * All rights reserved
 * Distributed under Apache 2.0
 */

#include <stdio.h>
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024
#define GL3W_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "common.h"

#include "main.h"
#include "mainscene.h"
#include "intro-scene.h"

#define GL_MATRIX_STACK_IMPLEMENTATION
#include "gl-matrix-stack.h"



GLFWwindow* window;
// show the nuklear GUIs or not.  pressing escape toggles
// it
bool guiEnable = true;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    //glfwSetWindowShouldClose(window, GLFW_TRUE);
    guiEnable = !guiEnable;
  }
}





#ifdef __cplusplus
extern "C"
#endif
int
main(int argc, char** argv)
{
  glfwSetErrorCallback(error_callback);
  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  /* Create a windowed mode window and its OpenGL context */
  if(!(window = glfwCreateWindow(640,
                                 480,
                                 "Hello World",
                                 NULL,
                                 NULL)))
    {
      glfwTerminate();
      return -1;
    }
  glfwSetKeyCallback(window, key_callback);


  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  // init GL3W
  {
    if (gl3w_init())
      {
        printf("Could not init gl3w\n");
        return 1;
      }
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


  // set viewport
  {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0,
               width, height);
  }


  // get controllers
  init_controllers();


  struct scene_callbacks intro_scene_callbacks = {
    .init_scene = intro_scene_init_scene,
    .leave_scene = intro_scene_leave_scene,
    .draw_scene = intro_scene_draw_scene,
    .window_size_callback = intro_scene_window_size_callback,
    .draw_nuklear = intro_scene_draw_nuklear,
  };
  // use the intro scene's callbacks
  struct scene_callbacks current_scene = intro_scene_callbacks;
  // init the intro scene
  (*current_scene.init_scene)();


  struct scene_callbacks main_scene_callbacks = {
    .init_scene = main_scene_init_scene,
    .leave_scene = main_scene_leave_scene,
    .draw_scene = main_scene_draw_scene,
    .window_size_callback = main_scene_window_size_callback,
    .draw_nuklear = main_scene_draw_nuklear,
  };
  // init the main scene
  (*main_scene_callbacks.init_scene)();




  // nuklear context
  struct nk_context *ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
  {
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {
      struct nk_font_atlas *atlas;
      nk_glfw3_font_stash_begin(&atlas);
      /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
      /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16, 0);*/
      /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
      /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
      /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
      /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
      nk_glfw3_font_stash_end();
      /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
      /*nk_style_set_font(ctx, &roboto->handle)*/;}

    /* style.c */
    /*set_style(ctx, THEME_WHITE);*/
    /*set_style(ctx, THEME_RED);*/
    /*set_style(ctx, THEME_BLUE);*/
    /*set_style(ctx, THEME_DARK);*/

  }

  // The event loop.  Keep on truckin'.
  while (!glfwWindowShouldClose(window))
    {

      /* Poll for and process events */
      glfwPollEvents();

      if(guiEnable) nk_glfw3_new_frame();


      (*current_scene.draw_scene)();

      // nuklear
      if(guiEnable)
      {
	/* IMPORTANT: `nk_glfw3_render` modifies some global OpenGL state
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
	nk_glfw3_render(NK_ANTI_ALIASING_ON,
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
      glfwSwapBuffers(window);

    }

  // Cleanup
  glfwTerminate();
  return 0;
}
