#include "common.h"

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


void
init_controllers()
{
  // TODO -- create a data structure to handle the controllers
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
  for (int8_t i = 0; i < SDL_NumJoysticks(); ++i) {
    if (SDL_IsGameController(i)) {
      controller = SDL_GameControllerOpen(i);
        if (controller) {
          SDL_Joystick *joy = SDL_GameControllerGetJoystick( controller );
          int8_t instanceID = SDL_JoystickInstanceID( joy );
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
