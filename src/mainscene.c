/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include "common.h"
#include "shader.h"
#include "main.h"

struct camera camera = {
  .x = 0.0,
  .y = 5.0,
  .z = 0.0,
  .rotationX = 0.0,
  .rotationY = 0.0
};

enum VAO_IDS{
  WALLS,
  NumVAOS
};
enum Buffer_IDS{
  Position,
  UV,
  NumBuffers
};
enum Attribute_IDS{
  vPosition = 0,
  vUV = 1
};

static GLuint
VAOs[NumVAOS];

static GLuint
Buffers[NumBuffers];

GLuint textureID;

static float
projection_matrix[16];

static float
model_view_matrix[16];

static GLuint
wallsProgramID;

float
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


const GLchar *
vertex_shader =
  "#version 330 core                                       \n"
  "layout (location = 0) in vec3 vPosition;                \n"
  "layout (location = 1) in vec2 vUV;                      \n"
  "                                                        \n"
  "out VS_OUT {                                            \n"
  "  vec2 uv;                                              \n"
  "} vs_out;                                               \n"
  "                                                        \n"
  "uniform mat4 mvpMatrix;                                 \n"
  "                                                        \n"
  "void                                                    \n"
  "main(){                                                 \n"
  "   vs_out.uv = vUV;                                     \n"
  "   gl_Position = mvpMatrix * vec4(vPosition,1.0);       \n"
  "}";


const GLchar *
fragment_shader =
  "#version 330 core                                           \n"
  "                                                            \n"
  "in VS_OUT {                                                 \n"
  "  vec2 uv;                                                  \n"
  "} fs_in;                                                    \n"
  "uniform sampler2D myTextureSampler;                         \n"
  "                                                            \n"
  "out vec3 color;                                             \n"
  "                                                            \n"
  "void main(){                                                \n"
  "  color = texture( myTextureSampler, fs_in.uv * 20 ).rgb;   \n"
  "}";




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
  }

  // load shaders
  {
    const GLuint vertexShaderID = compile_shader(GL_VERTEX_SHADER,
                                                 &vertex_shader);
    const GLuint fragmentShaderID = compile_shader(GL_FRAGMENT_SHADER,
                                                   &fragment_shader);
    wallsProgramID = link_shaders(vertexShaderID,fragmentShaderID);

    // clean up
    glDeleteShader(vertexShaderID);
    GL_DEBUG_ASSERT();
    glDeleteShader(fragmentShaderID);
    GL_DEBUG_ASSERT();
  }

  // initialize projection matrix
  {
    mat4_identity(projection_matrix);
    int w, h;
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
    SDL_Surface *thegrid = IMG_Load("../share/hurtbox/thegrid.png");
    //puts(SDL_GetError());


    const GLuint mode = thegrid->format->BytesPerPixel == 4
      ? GL_RGBA
      : GL_RGB;


    // Create one OpenGL texture
    {
      glGenTextures(1, &textureID);
      glBindTexture(GL_TEXTURE_2D, textureID);
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      glPixelStorei(GL_PACK_ALIGNMENT,1);


      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
      /* glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE); */
      /* glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE); */
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   mode,
                   thegrid->w,
                   thegrid->h,
                   0,
                   mode,
                   GL_UNSIGNED_BYTE,
                   thegrid->pixels);
    }
    SDL_FreeSurface(thegrid);
  }
}


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
    float neg_camera[] = {-camera.x,-camera.y,-camera.z};
    mat4_translate(wall_model_view_matrix,
                   neg_camera,
                   NULL);
  }

  //draw the four walls
  {
    const float pi_over_two = 1.57079632679;
    float rotation = 0.0f;
    for(int i = 0;
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

        glUniformMatrix4fv(glGetUniformLocation(wallsProgramID,
                                                "mvpMatrix"),
                           1,
                           GL_FALSE,
                           model_view_matrix);
        GL_DEBUG_ASSERT();


        // set the vertex data
        {
          glEnableVertexAttribArray(vPosition);
          GL_DEBUG_ASSERT();
          glBindBuffer(GL_ARRAY_BUFFER, Buffers[Position]);
          GL_DEBUG_ASSERT();
          glVertexAttribPointer(
                                vPosition,
                                3,
                                GL_FLOAT,
                                GL_FALSE,
                                0,
                                (void*)0
                                );
          GL_DEBUG_ASSERT();
        }


        // set the UV data
        {

          GLuint textureUniform = glGetUniformLocation(wallsProgramID, "myTextureSampler");
          GL_DEBUG_ASSERT();

          // Bind our texture in Texture Unit 0
          glActiveTexture(GL_TEXTURE0);
          GL_DEBUG_ASSERT();
          glUniform1i(textureUniform, 0);
          GL_DEBUG_ASSERT();
          glBindTexture(GL_TEXTURE_2D, textureID);
          GL_DEBUG_ASSERT();
          // Set our "myTextureSampler" sampler to user Texture Unit 0





          glEnableVertexAttribArray(vUV);
          GL_DEBUG_ASSERT();
          glBindBuffer(GL_ARRAY_BUFFER, Buffers[UV]);
          GL_DEBUG_ASSERT();
          glVertexAttribPointer(
                                vUV,
                                2,
                                GL_FLOAT,
                                GL_FALSE,
                                0,
                                (void*)0
                                );
          GL_DEBUG_ASSERT();
        }


        static GLuint numVertices = ARRAY_SIZE(wallVertices)/3;

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES,
                     0,
                     numVertices);
        GL_DEBUG_ASSERT();
        glDisableVertexAttribArray(vPosition);
        GL_DEBUG_ASSERT();
        glDisableVertexAttribArray(vUV);
        GL_DEBUG_ASSERT();
      }
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
  float* axisValue[] = {
    &left_axis.horizontal,
    &left_axis.vertical,
    &right_axis.horizontal,
    &right_axis.vertical
  };
  // the different axises should change the values at different scales
  float axisScale[] = {
    1.0,
    1.0,
    0.1,
    0.1
  };

  const int range = 32768;

  // linear scaling has to be changed, probably want x^2
  *axisValue[axis] = (value > -5000 && value < 5000)
    ? 0.0
    : -((float)value / (float)range) * axisScale[axis];
}


void
main_scene_handle_window_event(const SDL_Event* const event){
  switch (event->window.event){
  case SDL_WINDOWEVENT_RESIZED:
    {
      int w = event->window.data1, h = event->window.data2;
      mat4_perspective(45.0f,
                       (GLfloat)w / (GLfloat)h,
                       0.1f,
                       1000.0f,projection_matrix);
    }
    break;
  }
}




