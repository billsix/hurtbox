#ifdef __cplusplus
extern "C" {
#endif
  const float DEGREES_TO_RADIANS = 0.0174532925f;
  extern SDL_Window *window;
  extern SDL_Renderer *renderer;
  extern SDL_GLContext glcontext;
  struct camera{
    GLfloat x, y, z;
    GLfloat rotationX, rotationY;
  };
  extern struct camera camera;

  void initGL();
  int initSDL();
  void drawScene();
  extern void handleKey(SDL_Keycode *sym);
  extern void handleWindowEvent(SDL_Event *event);

  // test data
  extern float wall1Color[3];
  extern float wall2Color[3];
  extern float wall3Color[3];
  extern float wall4Color[3];
#ifdef __cplusplus
}
#endif
