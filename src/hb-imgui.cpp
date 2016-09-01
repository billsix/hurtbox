/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
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
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include "common.h"
#include "main.h"
#include "mainscene.h"
#include "hb-imgui.h"

bool
show_test_window = false;

bool
show_another_window = false;

bool
show_wall_color_chooser = false;

void
imgui_init()
{
  ImGui_ImplSdlGL3_Init(window);
  // Load Fonts
  // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
  //ImGuiIO& io = ImGui::GetIO();
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
}

void
imgui_process_events(SDL_Event *event)
{
  ImGui_ImplSdlGL3_ProcessEvent(event);
}

SDL_bool
imgui_wants_event()
{
  return (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput)
    ? SDL_TRUE
    : SDL_FALSE;
}

void
imgui_shutdown()
{
  ImGui_ImplSdlGL3_Shutdown();
}

void
drawIMGUI()
{
  ImGui_ImplSdlGL3_NewFrame(window);

  // 1. Show a simple window
  // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
  {
    static float f = 0.0f;
    ImGui::Text("Hello, world!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
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


  if (show_wall_color_chooser)
    {
      ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiSetCond_FirstUseEver);
      ImGui::SetNextWindowSize(ImVec2(400,200), ImGuiSetCond_FirstUseEver);

      ImGui::Begin("Wall Color Changer", &show_wall_color_chooser);

      // only flush the colors to the graphics card if a color is changed
      bool colorUpdated = false;
      // there are four wall
      for (Uint32 wall = 0; wall < 4; wall++)
        {
          // text to show up within the widget
          char wallName[] = "Wall1";
          // replace the 1 with the correct number (there are only four walls
          // so this simplistict method is fine
          wallName[4] = '1' + wall;

          // ask IMGUI to make a color editor
          // each wall
          {
            const int verticesPerTriangle = 3;
            const int trianglesPerWall = 2;
            const int gLFloatsPerVertex = 3;
            const int glFloatsPerWall = gLFloatsPerVertex * verticesPerTriangle * trianglesPerWall;

            if (ImGui::ColorEdit3(wallName, &wallColors[wall * glFloatsPerWall])) {
              colorUpdated = true;
              for (Uint32 i = 0; i < glFloatsPerWall; i += verticesPerTriangle)
                {
                  float* const colorToSet = &wallColors[wall * glFloatsPerWall + i];
                  const float* const value = &wallColors[wall * glFloatsPerWall];
                  memcpy(colorToSet, value, 3 * sizeof(float));
                }
            }
          }
        }
      if (colorUpdated)
        main_scene_update_wall_colors();
      ImGui::End();

    }

  const bool show_app_main_menu_bar = true;

  if(show_app_main_menu_bar){
    if (ImGui::BeginMainMenuBar())
      {
        if (ImGui::BeginMenu("Hurtbox"))
          {
            {
              if (ImGui::MenuItem("Show Wall Color Chooser")) {
                show_wall_color_chooser = true;
              }
              if (ImGui::MenuItem("Show Test Window")) {
                show_test_window = true;
              }

            }
            ImGui::EndMenu();
          }

        // if (ImGui::BeginMenu("Edit"))
        //   {
        //     if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        //     if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        //     ImGui::Separator();
        //     if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        //     if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        //     if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        //     ImGui::EndMenu();
        //   }
        ImGui::EndMainMenuBar();
      }
  }

  // Rendering
  glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
  ImGui::Render();
}
