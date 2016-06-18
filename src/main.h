const int SCREEN_WIDTH  = 1000;
const int SCREEN_HEIGHT = 1000;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_GLContext glcontext;
SDL_bool quitMainLoop = SDL_FALSE;
int returnCode = 0;


// temporary to test opengl with imgui
double rotation = 0.0;
