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
#include "imgui_impl_sdl.h"
#include "common.h"
#include "main.h"
#include "hb-imgui.h"

bool  show_test_window = false;
bool  show_another_window = false;
bool show_wall_color_chooser = false;

void
drawIMGUI()
{
  ImGui_ImplSdl_NewFrame(window);

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
      ImGui::SetNextWindowSize(ImVec2(550,680), ImGuiSetCond_FirstUseEver);

      ImGui::Begin("Wall Color Changer", &show_wall_color_chooser);

      ImGui::ColorEdit3("Wall1", wall1Color);
      ImGui::ColorEdit3("Wall2", wall2Color);
      ImGui::ColorEdit3("Wall3", wall3Color);
      ImGui::ColorEdit3("Wall4", wall4Color);
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

void
imgui_init()
{
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
}

void
imgui_process_events(SDL_Event *event)
{
  ImGui_ImplSdl_ProcessEvent(event);
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
  ImGui_ImplSdl_Shutdown();
}
