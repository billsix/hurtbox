/*
 * William Emerison Six
 *
 * Copyright 2014-2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */


/*
 * Because of content copied from contrib/imgui/examples/sdl_opengl_example/main.cpp,
 * I must include the following copyright from Omar Cornut and ImGui contributors.
 */

/*
 * The MIT License (MIT)
 *
 *Copyright (c) 2014-2015 Omar Cornut and ImGui contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "SDL2/SDL.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include "../contrib/imgui/imgui.h"
#include "../contrib/imgui/examples/sdl_opengl_example/imgui_impl_sdl.h"

#include "main.h"
#include <stdio.h>


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


void drawIMGUI()
{
  ImGui_ImplSdl_NewFrame(window);

  // 1. Show a simple window
  // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
  {
    static float f = 0.0f;
    ImGui::Text("Hello, world!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float*)&clear_color);
    if (ImGui::Button("Test Window")) show_test_window ^= 1;
    if (ImGui::Button("Another Window")) show_another_window ^= 1;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }

  // 2. Show another simple window, this time using an explicit Begin/End pair
  if (show_another_window)
    {
      ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello");
      ImGui::End();
    }

  // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
  if (show_test_window)
    {
      ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
      ImGui::ShowTestWindow(&show_test_window);
    }

  // Rendering
  glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
  ImGui::Render();
}

int main(int, char**)
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

  // Setup ImGui binding
  ImGui_ImplSdl_Init(window);

  // Load Fonts
  // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
  //ImGuiIO& io = ImGui::GetIO();
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());


  // Main loop
  bool done = false;
  while (!done)
    {

      SDL_Event event;
      while (SDL_PollEvent(&event))
        {
          ImGui_ImplSdl_ProcessEvent(&event);
          if (event.type == SDL_QUIT){
            done = true;
          }
          if(ImGui::GetIO().WantCaptureMouse
             || ImGui::GetIO().WantTextInput){
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

      glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      drawScene();
      drawIMGUI();
      SDL_GL_SwapWindow(window);
    }

  // Cleanup
  ImGui_ImplSdl_Shutdown();
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

