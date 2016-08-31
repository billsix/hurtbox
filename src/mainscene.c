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


struct camera camera = { .x = 0.0,
                         .y = 5.0,
                         .z = 0.0,
                         .rotationX = 0.0,
                         .rotationY = 0.0};

enum VAO_IDS{WALLS,NumVAOS};
enum Buffer_IDS{Position, Color, NumBuffers};
enum Attribute_IDS{ vPosition = 0, vColor = 1};

static GLuint VAOs[NumVAOS];//VertexArrayID;
static GLuint Buffers[NumBuffers];


float wallVertices[] =
  {
    -40.0, 0.0, -40.0,
    40.0, 0.0, -40.0,
    40.0, 30.0, -40.0,

    40.0, 30.0, -40.0,
    -40.0, 30.0, -40.0,
    -40.0, 0.0, -40.0,

    -40.0, 0.0, 40.0,
    -40.0, 30.0, 40.0,
    40.0, 30.0, 40.0,

    40.0, 30.0, 40.0,
    40.0, 0.0, 40.0,
    -40.0, 0.0, 40.0,

    -40.0, 0.0, -40.0,
    -40.0, 30.0, -40.0,
    -40.0, 30.0, 40.0,

    -40.0, 30.0, 40.0,
    -40.0, 0.0, 40.0,
    -40.0, 0.0, -40.0,

    40.0, 30.0, -40.0,
    40.0, 0.0, -40.0,
    40.0, 0.0, 40.0,

    40.0, 0.0, 40.0,
    40.0, 30.0, 40.0,
    40.0, 30.0, -40.0
  };

float wallColors[] = {
  1.0,1.0,1.0,
  1.0,1.0,1.0,
  1.0,1.0,1.0,

  1.0,1.0,1.0,
  1.0,1.0,1.0,
  1.0,1.0,1.0,

  0.0,1.0,1.0,
  0.0,1.0,1.0,
  0.0,1.0,1.0,

  0.0,1.0,1.0,
  0.0,1.0,1.0,
  0.0,1.0,1.0,

  0.0,0.0,1.0,
  0.0,0.0,1.0,
  0.0,0.0,1.0,

  0.0,0.0,1.0,
  0.0,0.0,1.0,
  0.0,0.0,1.0,

  1.0,0.0,1.0,
  1.0,0.0,1.0,
  1.0,0.0,1.0,

  1.0,0.0,1.0,
  1.0,0.0,1.0,
  1.0,0.0,1.0
};




const GLchar * vertex_shader =
  "#version 330 core                           \n"
  "layout (location = 0) in vec3 vPosition;    \n"
  "layout (location = 1) in vec3 vColor;       \n"
  "                                            \n"
  "out vec4 VOcolor;                           \n"
  "                                            \n"
  "uniform mat4 mvpMatrix;                     \n"
  "                                            \n"
  "void                                        \n"
  "main(){                                     \n"
  "   VOcolor.xyz = vColor;                    \n"
  "   VOcolor.w = 1.0;                         \n"
  "   vec4 pos;                                \n"
  "   pos.xyz = vPosition;                     \n"
  "   pos.w = 1.0;                             \n"
  "   gl_Position = mvpMatrix * pos;           \n"
  "}";


const GLchar * fragment_shader =
  "#version 330 core                          \n"
  "in vec4 VOcolor;                           \n"
  "out vec4 color;                            \n"
  "                                           \n"
  "void main(){                               \n"
  "   color = VOcolor;                        \n"
  "}";





static GLuint
wallsProgramID;

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
      glBindBuffer(GL_ARRAY_BUFFER, Buffers[Color]);
      GL_DEBUG_ASSERT();
      glBufferData(GL_ARRAY_BUFFER,
                   sizeof(wallColors),
                   wallColors,
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
}

void
main_scene_update_wall_colors()
{
  glBindVertexArray(VAOs[WALLS]);
  GL_DEBUG_ASSERT();

  glBindBuffer(GL_ARRAY_BUFFER, Buffers[Color]);
  GL_DEBUG_ASSERT();

  glBufferData(GL_ARRAY_BUFFER,
               sizeof(wallColors),
               wallColors,
               GL_STATIC_DRAW);
  GL_DEBUG_ASSERT();
}

const GLuint
numVertices = ARRAY_SIZE(wallVertices) / 3;

                                       void
                                       main_scene_draw_scene(const Uint8 * const state)
{
  glUseProgram(wallsProgramID);
  GL_DEBUG_ASSERT();

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

  GLfloat cameraMatrix[16];
  mat4_identity(cameraMatrix);

  // translate from world coordinates to camera coordinates
  {
    mat4_rotateX(cameraMatrix,
                 camera.rotationX,
                 cameraMatrix);
    mat4_rotateY(cameraMatrix,
                 -camera.rotationY,
                 cameraMatrix);
    float neg_camera[] = {-camera.x,-camera.y,-camera.z};
    mat4_translate(cameraMatrix,
                   neg_camera,
                   cameraMatrix);
  }

  float current_matrix[16];
  //draw the four walls
  {
    mat4_multiply(projection_matrix,
                  cameraMatrix,
                  current_matrix);

    glUniformMatrix4fv(glGetUniformLocation(wallsProgramID,
                                            "mvpMatrix"),
                       1,
                       GL_FALSE,
                       current_matrix);
    GL_DEBUG_ASSERT();


    // set the vertex data
    {
      glEnableVertexAttribArray(vPosition);
      glBindBuffer(GL_ARRAY_BUFFER, Buffers[Position]);
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

    // set the color data
    {
      glEnableVertexAttribArray(vColor);
      GL_DEBUG_ASSERT();
      glBindBuffer(GL_ARRAY_BUFFER, Buffers[Color]);
      GL_DEBUG_ASSERT();
      glVertexAttribPointer(
                            vColor,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            0,
                            (void*)0
                            );
      GL_DEBUG_ASSERT();
    }



    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES,
                 0,
                 numVertices);
    GL_DEBUG_ASSERT();
    glDisableVertexAttribArray(vPosition);
    GL_DEBUG_ASSERT();
    glDisableVertexAttribArray(vColor);
    GL_DEBUG_ASSERT();
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
  float* axisValue[] = { &left_axis.horizontal,
                         &left_axis.vertical,
                         &right_axis.horizontal,
                         &right_axis.vertical };
  // the different axises should change the values at different scales
  float axisScale[] = { 1.0,1.0,0.1,0.1 };

  const int range = 32768;

  for (int i = 0; i < ARRAY_SIZE(axisValue); i++) {
    if (axis == i) {
      // linear scaling has to be changed, probably want x^2
      *axisValue[i] = (value > -5000 && value < 5000)
        ? 0.0
        : -((float)value / (float)range) * axisScale[i];
    }
  }
}



