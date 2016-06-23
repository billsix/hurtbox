/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#ifdef __cplusplus
extern "C" {
#endif
  void drawIMGUI();
  void imgui_init();
  void imgui_process_events(SDL_Event *event);
  SDL_bool imgui_wants_event();
  void imgui_shutdown();
  void drawIMGUI(); 
#ifdef __cplusplus
}
#endif
