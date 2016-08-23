/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include "common.h"
#include "main.h"


struct camera camera = { .x = 0.0,
                         .y = 5.0,
                         .z = 0.0,
                         .rotationX = 0.0,
                         .rotationY = 0.0};

// test data
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


static GLuint vertexbuffer;
static GLuint colorbuffer;


const GLchar * const vertex_shader =
  "#version 150 core \n"
  "in vec4 vPosition;   \n"
  "in vec4 vColor; \n"
  "\n"
  "out vec4 VOcolor; \n"
  "\n"
  "uniform mat4 mvpMatrix; \n"
  "\n"
  "void \n"
  "main(){ \n"
  "VOcolor = vColor; \n"
  "gl_Position = mvpMatrix * vPosition; \n"
  "}";


const GLchar * const fShader =
  "#version 150 core \n"
  "in vec4 VOcolor; \n"
  "out vec4 color; \n"
  "\n"
  "void main(){ \n"
  "color = VOcolor; \n"
  "}";


GLuint ProgramID;

void
main_scene_init_scene()
{

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  GL_DEBUG_ASSERT();
  glBindVertexArray(VertexArrayID);
  GL_DEBUG_ASSERT();

  // populate vertex buffer
  {
    // This will identify our vertex buffer
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    GL_DEBUG_ASSERT();
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER,
                 vertexbuffer);
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
    glGenBuffers(1, &colorbuffer);
    GL_DEBUG_ASSERT();
    glBindBuffer(GL_ARRAY_BUFFER,
                 colorbuffer);
    GL_DEBUG_ASSERT();
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(wallColors),
                 wallColors,
                 GL_STATIC_DRAW);
    GL_DEBUG_ASSERT();
  }




  GLint Result = GL_FALSE;
  int InfoLogLength;
  {
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GL_DEBUG_ASSERT();
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GL_DEBUG_ASSERT();


    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		   SDL_LOG_PRIORITY_INFO,
		   "Compiling Vertex shader : %s\n",
		   vertex_shader);

    // Compile Vertex Shader
    glShaderSource(VertexShaderID, 1, &vertex_shader , NULL);
    GL_DEBUG_ASSERT();
    glCompileShader(VertexShaderID);
    GL_DEBUG_ASSERT();

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    GL_DEBUG_ASSERT();
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    GL_DEBUG_ASSERT();
    if ( InfoLogLength > 0 ){
      char *foo = (char*) malloc(InfoLogLength * sizeof(char));
      glGetShaderInfoLog(VertexShaderID,
                         InfoLogLength,
                         NULL,
                         foo);
      GL_DEBUG_ASSERT();
      SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		     SDL_LOG_PRIORITY_INFO,
		     "Vertex Shader info %s\n",
		     foo);
      free(foo);
    }



    // Compile Fragment Shader
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		   SDL_LOG_PRIORITY_INFO,
		   "Compiling Fragment shader : %s\n",
		   fShader);
    glShaderSource(FragmentShaderID, 1, &fShader , NULL);
    GL_DEBUG_ASSERT();
    glCompileShader(FragmentShaderID);
    GL_DEBUG_ASSERT();

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    GL_DEBUG_ASSERT();
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    GL_DEBUG_ASSERT();
    if ( InfoLogLength > 0 ){

      char *foo = (char*) malloc(InfoLogLength * sizeof(char));
      glGetShaderInfoLog(FragmentShaderID,
                         InfoLogLength,
                         NULL,
                         foo);
      GL_DEBUG_ASSERT();
      SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		     SDL_LOG_PRIORITY_INFO,
		     "Fragment Shader Info %s\n",
		     foo);
      free(foo);
    }



    // Link the program
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		   SDL_LOG_PRIORITY_INFO,
		   "Linking program\n");

    ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    GL_DEBUG_ASSERT();
    glAttachShader(ProgramID, FragmentShaderID);
    GL_DEBUG_ASSERT();
    glLinkProgram(ProgramID);
    GL_DEBUG_ASSERT();

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    GL_DEBUG_ASSERT();
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    GL_DEBUG_ASSERT();
    if ( InfoLogLength > 0 ){
      SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		     SDL_LOG_PRIORITY_INFO,
		     "log len %d %d\n",
		     InfoLogLength,
		     Result);
      char *foo = (char*) malloc(InfoLogLength * sizeof(char));
      glGetShaderInfoLog(ProgramID,
			 InfoLogLength,
			 NULL,
                         foo);
      GL_DEBUG_ASSERT();
      SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		     SDL_LOG_PRIORITY_INFO,
		     "Linking info %s\n",
		     foo);
      free(foo);

    }


    glDetachShader(ProgramID, VertexShaderID);
    GL_DEBUG_ASSERT();
    glDetachShader(ProgramID, FragmentShaderID);
    GL_DEBUG_ASSERT();

    glDeleteShader(VertexShaderID);
    GL_DEBUG_ASSERT();
    glDeleteShader(FragmentShaderID);
    GL_DEBUG_ASSERT();

  }

}




void
main_scene_draw_scene(const Uint8 *state)
{
  glUseProgram(ProgramID);
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

    glUniformMatrix4fv(glGetUniformLocation(ProgramID,
                                            "mvpMatrix"),
                       1,
                       GL_FALSE,
                       current_matrix);
    GL_DEBUG_ASSERT();


    // set the vertex data
    {
      glEnableVertexAttribArray(glGetAttribLocation(ProgramID,
                                                    "vPosition"));
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      glVertexAttribPointer(
                            0,
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
      glEnableVertexAttribArray(glGetAttribLocation(ProgramID,
                                                    "vColor"));
      GL_DEBUG_ASSERT();
      glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
      GL_DEBUG_ASSERT();
      glVertexAttribPointer(
                            1,
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
                 ARRAY_SIZE(wallVertices)/3);
    GL_DEBUG_ASSERT();
    glDisableVertexAttribArray(0);
    GL_DEBUG_ASSERT();
    glDisableVertexAttribArray(1);
    GL_DEBUG_ASSERT();
  }

}


void
main_scene_controller_handle_button (SDL_ControllerButtonEvent sdlEvent)
{

}


void
main_scene_controller_handle_axis(SDL_ControllerAxisEvent controllerAxisEvent){
  Uint32 timestamp	= controllerAxisEvent.timestamp;
  SDL_JoystickID which = controllerAxisEvent.which;
  Uint8 axis = controllerAxisEvent.axis;
  Sint16 value = controllerAxisEvent.value; //the axis value (range: -32768 to 32767)

  if(axis == 0){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      left_axis.horizontal = 0.0;
    }
    else{
      left_axis.horizontal = -((float)value / (float)range);
    }
  }
  else if(axis == 1){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      left_axis.vertical = 0.0;
    }
    else{
      left_axis.vertical = -((float)value / (float)range) ;
    }
  }
  else if(axis == 2){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      right_axis.horizontal = 0.0;
    }
    else{
      right_axis.horizontal = -((float)value / (float)range) * 0.1;
    }
  }
  else if(axis == 3){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      right_axis.vertical = 0.0;
    }
    else{
      right_axis.vertical = -((float)value / (float)range) * 0.1;
    }
  }
}
