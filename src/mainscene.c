/*
 * William Emerison Six
 *
 * Copyright 2016-2017 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include "common.h"
#include "main.h"

#include "shader.h"
#include "mainscene.h"
#include "gl-matrix.h"




/*
 * projection and modelview matricies
 * TODO - make these stackable, with a current matrix
 */

static float
projection_matrix[16];

static float
model_view_matrix[16];




/*
 * The vertex and UV data
 */

GLfloat
wallVertices[] =
  {
    // triangle 1
    -40.0, 0.0, -40.0,
    40.0, 0.0, -40.0,
    40.0, 30.0, -40.0,
    // triangle 2
    40.0, 30.0, -40.0,
    -40.0, 30.0, -40.0,
    -40.0, 0.0, -40.0
  };

GLfloat
wallUVs[] = {
  // triangle 1
  0.0,0.0,
  1.0,0.0,
  1.0,1.0,
  // triangle 2
  1.0,1.0,
  0.0,1.0,
  0.0,0,0
};



enum VAO_IDS{
  WALLS,
  NumVAOS // calculated at compile-time, someone should write a book about that
};

static GLuint
VAOs[NumVAOS];

enum Buffer_IDS{
  Position,
  UV,
  NumBuffers // calculated at compile-time, someone should write a book about that
};

static GLuint
Buffers[NumBuffers];


static GLuint
wallsProgramID;

static GLuint
mvpMatrixLoc;

static GLuint
textureID;

static GLuint
wallTextureLoc;

void
main_scene_init_scene()
{

  glGenVertexArrays(NumVAOS, VAOs);
  GL_DEBUG_ASSERT();



  glBindVertexArray(VAOs[WALLS]);
  GL_DEBUG_ASSERT();

  // populate the buffers
  glGenBuffers(NumBuffers, Buffers);
  {
    // populate vertex buffer
    {
      // This will identify our vertex buffer
      // Generate 1 buffer, put the resulting identifier in vertexbuffer
      GL_DEBUG_ASSERT();
      // The following commands will talk about our 'vertexbuffer' buffer
      glBindBuffer(GL_ARRAY_BUFFER, Buffers[Position]);
      GL_DEBUG_ASSERT();
      // Give our vertices to OpenGL.
      glBufferData(GL_ARRAY_BUFFER,
                   sizeof(wallVertices),
                   wallVertices,
                   GL_STATIC_DRAW);
      GL_DEBUG_ASSERT();
    }

    // populate color buffer
    {
      GL_DEBUG_ASSERT();
      glBindBuffer(GL_ARRAY_BUFFER, Buffers[UV]);
      GL_DEBUG_ASSERT();
      glBufferData(GL_ARRAY_BUFFER,
                   sizeof(wallUVs),
                   wallUVs,
                   GL_STATIC_DRAW);
      GL_DEBUG_ASSERT();
    }

    enum Attribute_IDS{
      vPosition = 0,
      vUV = 1
    };


    // set the vertex data
    {
      glEnableVertexAttribArray(vPosition);
      GL_DEBUG_ASSERT();
      glBindBuffer(GL_ARRAY_BUFFER, Buffers[Position]);
      GL_DEBUG_ASSERT();
      glVertexAttribPointer(vPosition,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            0,
                            BUFFER_OFFSET(0));
      GL_DEBUG_ASSERT();
    }

    // set the UV data
    {
      GL_DEBUG_ASSERT();

      glEnableVertexAttribArray(vUV);
      GL_DEBUG_ASSERT();
      glBindBuffer(GL_ARRAY_BUFFER, Buffers[UV]);
      GL_DEBUG_ASSERT();
      glVertexAttribPointer(vUV,
                            2,
                            GL_FLOAT,
                            GL_FALSE,
                            0,
                            BUFFER_OFFSET(0));
      GL_DEBUG_ASSERT();
    }


  }

  // load shaders
  {

    GLuint vertexShaderID = compile_shader(GL_VERTEX_SHADER,
                                           SHADER_DIR "walls.vert");
    
    GLuint fragmentShaderID =compile_shader(GL_FRAGMENT_SHADER,
                                            SHADER_DIR "walls.frag");

    wallsProgramID = link_shaders(vertexShaderID,fragmentShaderID);

    mvpMatrixLoc = glGetUniformLocation(wallsProgramID,
                                        "mvpMatrix");
    wallTextureLoc = glGetUniformLocation(wallsProgramID,
                                          "wallTexture");

    // clean up
    glDeleteShader(vertexShaderID);
    GL_DEBUG_ASSERT();
    glDeleteShader(fragmentShaderID);
    GL_DEBUG_ASSERT();
  }

  // initialize projection matrix
  {
    mat4_identity(projection_matrix);
    int32_t w, h;
    SDL_GetWindowSize(window,&w,&h);
    mat4_perspective(45.0f,
                     (GLfloat)w / (GLfloat)h,
                     0.1f,
                     1000.0f,
                     projection_matrix);

  }
  //initialize the modelview matrix
  {
    mat4_identity(model_view_matrix);
  }


  // load textures
  {

    SDL_Surface *thegrid = IMG_Load(TEXTURE_DIR "thegrid.png");
    //puts(SDL_GetError());


    const GLuint mode = thegrid->format->BytesPerPixel == 4
      ? GL_RGBA
      : GL_RGB;


    // Create one OpenGL texture
    {
      glGenTextures(1, &textureID);
      GL_DEBUG_ASSERT();
      glBindTexture(GL_TEXTURE_2D, textureID);
      GL_DEBUG_ASSERT();
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      GL_DEBUG_ASSERT();
      glPixelStorei(GL_PACK_ALIGNMENT,1);
      GL_DEBUG_ASSERT();


      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      GL_DEBUG_ASSERT();
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      GL_DEBUG_ASSERT();
      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
      GL_DEBUG_ASSERT();
      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
      GL_DEBUG_ASSERT();
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   mode,
                   thegrid->w,
                   thegrid->h,
                   0,
                   mode,
                   GL_UNSIGNED_BYTE,
                   thegrid->pixels);
      GL_DEBUG_ASSERT();
      GL_DEBUG_ASSERT();
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      GL_DEBUG_ASSERT();
      glPixelStorei(GL_PACK_ALIGNMENT, 1);
      GL_DEBUG_ASSERT();
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    SDL_FreeSurface(thegrid);
  }
}

void
main_scene_leave_scene()
{

}


/*
 * camera data
 */

struct camera camera = {
  .x = 0.0,
  .y = 5.0,
  .z = 0.0,
  .rotationX = 0.0,
  .rotationY = 0.0
};


void
main_scene_draw_scene(const Uint8 * const state)
{
  // update camera from the controller
  {
    camera.x -= ( GLfloat ) sin( camera.rotationY) * left_axis.vertical;
    camera.z -= ( GLfloat ) cos( camera.rotationY) * left_axis.vertical;

    camera.x -= ( GLfloat ) cos(camera.rotationY) * left_axis.horizontal;
    camera.z += ( GLfloat ) sin(camera.rotationY) * left_axis.horizontal;

    camera.rotationX += right_axis.vertical;
    camera.rotationY += right_axis.horizontal;

  }

  // update camera from the keyboard
  {
    if (state[SDL_SCANCODE_RIGHT]) {
      camera.rotationY -= (GLfloat)0.03;
    }
    if (state[SDL_SCANCODE_LEFT]) {
      camera.rotationY += (GLfloat)0.03;
    }
    if (state[SDL_SCANCODE_UP]) {
      camera.x -= (GLfloat)sin(camera.rotationY);
      camera.z -= (GLfloat)cos(camera.rotationY);
    }
    if (state[SDL_SCANCODE_DOWN]) {
      camera.x += (GLfloat)sin(camera.rotationY);
      camera.z += (GLfloat)cos(camera.rotationY);
    }
  }


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_DEBUG_ASSERT();


  glUseProgram(wallsProgramID);
  GL_DEBUG_ASSERT();


  GLfloat wall_model_view_matrix[16];
  mat4_identity(wall_model_view_matrix);

  // translate from object coordinates to camera coordinates
  {
    mat4_rotateX(wall_model_view_matrix,
                 camera.rotationX,
                 NULL);
    mat4_rotateY(wall_model_view_matrix,
                 -camera.rotationY,
                 NULL);
    GLfloat neg_camera[] = {-camera.x,-camera.y,-camera.z};
    mat4_translate(wall_model_view_matrix,
                   neg_camera,
                   NULL);
  }

  //draw the four walls
  {
    glBindVertexArray(VAOs[WALLS]);
    const GLfloat pi_over_two = 1.57079632679;
    GLfloat rotation = 0.0f;
    for(uint8_t i = 0;
        i<4;
        rotation+=pi_over_two,i++)
      {

        mat4_rotateY(wall_model_view_matrix,
                     rotation,
                     NULL);


        // translate from camera coordinates to screen coordinates
        mat4_multiply(projection_matrix,
                      wall_model_view_matrix,
                      model_view_matrix);

        glUniformMatrix4fv(mvpMatrixLoc,
                           1,
                           GL_FALSE,
                           model_view_matrix);
        GL_DEBUG_ASSERT();



        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        GL_DEBUG_ASSERT();
        glUniform1i(wallTextureLoc, 0);
        GL_DEBUG_ASSERT();
        glBindTexture(GL_TEXTURE_2D, textureID);
        GL_DEBUG_ASSERT();



        static GLuint numVertices = ARRAY_SIZE(wallVertices)/3;

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES,
                     0,
                     numVertices);
        GL_DEBUG_ASSERT();
      }
    // unbind the current VAO
    glBindVertexArray(0);

  }

}


void
main_scene_controller_handle_button (const SDL_ControllerButtonEvent * const sdlEvent)
{

}


void
main_scene_controller_handle_axis(const SDL_ControllerAxisEvent * const controllerAxisEvent){
  Uint32 timestamp	= controllerAxisEvent->timestamp;
  SDL_JoystickID which = controllerAxisEvent->which;
  Uint8 axis = controllerAxisEvent->axis;
  Sint16 value = controllerAxisEvent->value; //the axis value (range: -32768 to 32767)

  // one of the following values must be updated
  GLfloat* axisValue[] = {
    &left_axis.horizontal,
    &left_axis.vertical,
    &right_axis.horizontal,
    &right_axis.vertical
  };
  // the different axises should change the values at different scales
  GLfloat axisScale[] = {
    1.0,
    1.0,
    0.1,
    0.1
  };

  const uint32_t range = 32768;

  // linear scaling has to be changed, probably want x^2
  *axisValue[axis] = (value > -5000 && value < 5000)
    ? 0.0
    : -((GLfloat)value / (GLfloat)range) * axisScale[axis];
}


void
main_scene_handle_window_event(const SDL_Event* const event){
  switch (event->window.event){
  case SDL_WINDOWEVENT_RESIZED:
    {
      uint32_t w = event->window.data1, h = event->window.data2;
      mat4_perspective(45.0f,
                       (GLfloat)w / (GLfloat)h,
                       0.1f,
                       1000.0f,projection_matrix);
    }
    break;
  }
}

void
main_scene_draw_nuklear(struct nk_context *ctx){
  if (nk_begin(ctx,
               "Demo",
               nk_rect(50, 50, 200, 200),
	       NK_WINDOW_BORDER
               | NK_WINDOW_MOVABLE
               | NK_WINDOW_SCALABLE
	       | NK_WINDOW_CLOSABLE
               | NK_WINDOW_MINIMIZABLE
               | NK_WINDOW_TITLE))
    {
      nk_menubar_begin(ctx);
      nk_layout_row_begin(ctx,
                          NK_STATIC,
                          25,
                          2);
      nk_layout_row_push(ctx,
                         45);
      if (nk_menu_begin_label(ctx,
                              "FILE",
                              NK_TEXT_LEFT,
                              nk_vec2(120, 200))) {
	nk_layout_row_dynamic(ctx,
                              30,
                              1);
	nk_menu_item_label(ctx,
                           "OPEN",
                           NK_TEXT_LEFT);
	nk_menu_item_label(ctx,
                           "CLOSE",
                           NK_TEXT_LEFT);
	nk_menu_end(ctx);
      }
      nk_layout_row_push(ctx,
                         45);
      if (nk_menu_begin_label(ctx,
                              "EDIT",
                              NK_TEXT_LEFT,
                              nk_vec2(120, 200))) {
	nk_layout_row_dynamic(ctx,
                              30,
                              1);
	nk_menu_item_label(ctx,
                           "COPY",
                           NK_TEXT_LEFT);
	nk_menu_item_label(ctx,
                           "CUT",
                           NK_TEXT_LEFT);
	nk_menu_item_label(ctx,
                           "PASTE",
                           NK_TEXT_LEFT);
	nk_menu_end(ctx);
      }
      nk_layout_row_end(ctx);
      nk_menubar_end(ctx);

      enum {EASY, HARD};
      static uint8_t op = EASY;
      static uint32_t property = 20;
      nk_layout_row_static(ctx,
                           30,
                           80,
                           1);
      if (nk_button_label(ctx,
                          "button")){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                       SDL_LOG_PRIORITY_INFO,
                       "Button pressed\n");
      }
      nk_layout_row_dynamic(ctx,
                            30,
                            2);
      if (nk_option_label(ctx,
                          "easy",
                          op == EASY))
        op = EASY;
      if (nk_option_label(ctx,
                          "hard",
                          op == HARD))
        op = HARD;
      nk_layout_row_dynamic(ctx,
                            25,
                            1);
      nk_property_int(ctx,
                      "Compression:",
                      0,
                      &property,
                      100,
                      10,
                      1);
    }
  nk_end(ctx);

  /* -------------- EXAMPLES ---------------- */
  /*calculator(ctx);*/
  /*overview(ctx);*/
  /*node_editor(ctx);*/
  /* ----------------------------------------- */


}
