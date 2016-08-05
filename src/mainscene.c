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
float wall1Colors[] = {1.0,1.0,1.0};
/* float wall2Color[3] = {0.0,1.0,1.0}; */
/* float wall3Color[3] = {0.0,0.0,1.0}; */
/* float wall4Color[3] = {1.0,0.0,1.0}; */


double wallVertices[] =
  {-40.0, 0.0, -40.0,
   40.0, 0.0, -40.0,
   40.0, 30.0, -40.0,
   -40.0, 30.0, -40.0};


static GLuint vertexbuffer;


const GLchar * const vertex_shader =
  "#version 450 core \
in vec4 vPosition; \
in vec4 vColor; \
 \
out vec4 color; \
 \
uniform mat4 mvpMatrix; \
 \
void \
main(){ \
  color = vColor; \
  gl_Position = mvpMatrix * vPosition; \
}";


const GLchar * const fShader =
  "#version 450 core \
int vec4 fcolor; \
out vec4 color; \
 \
void main(){ \
  color = fcolor; \
}";


GLuint ProgramID;

void
main_scene_init_scene()
{

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // This will identify our vertex buffer
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);


  GLint Result = GL_FALSE;
  int InfoLogLength;
  {
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    // Compile Vertex Shader
    glShaderSource(VertexShaderID, 1, &vertex_shader , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      /* std::vector<char> VertexShaderErrorMessage(InfoLogLength+1); */
      /* glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]); */
      /* printf("%s\n", &VertexShaderErrorMessage[0]); */
    }



    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fShader);
    glShaderSource(FragmentShaderID, 1, &fShader , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      /* std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1); */
      /* glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]); */
      /* printf("%s\n", &FragmentShaderErrorMessage[0]); */
    }



    // Link the program
    printf("Linking program\n");
    ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      /* std::vector<char> ProgramErrorMessage(InfoLogLength+1); */
      /* glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]); */
      /* printf("%s\n", &ProgramErrorMessage[0]); */
    }


    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    //return ProgramID;
  }

}




void
main_scene_draw_scene()
{


  // update camera from the controller
  {
    camera.x -= ( GLdouble ) sin( camera.rotationY) * left_axis.vertical;
    camera.z -= ( GLdouble ) cos( camera.rotationY) * left_axis.vertical;

    camera.x -= ( GLdouble ) cos(camera.rotationY) * left_axis.horizontal;
    camera.z += ( GLdouble ) sin(camera.rotationY) * left_axis.horizontal;

    camera.rotationX += right_axis.vertical;
    camera.rotationY += right_axis.horizontal;

  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLdouble cameraMatrix[16];
  mat4_identity(cameraMatrix);

  // translate from world coordinates to camera coordinates
  {
    mat4_rotateX(cameraMatrix,
                camera.rotationX,
                cameraMatrix);
    mat4_rotateY(cameraMatrix,
                -camera.rotationY,
                cameraMatrix);
    double neg_camera[] = {-camera.x,-camera.y,-camera.z};
    mat4_translate(cameraMatrix,
                   neg_camera,
                   cameraMatrix);
  }

  double current_matrix[16];
  //draw the four walls
  {
    mat4_multiply(projection_matrix,
                  cameraMatrix,
                  current_matrix);

    glLoadMatrixd(current_matrix);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                          0,   // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,                  // size
                          GL_DOUBLE,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );
    // Draw the triangle !
    glDrawArrays(GL_QUADS, 0, 4); // Starting from vertex 0
    glDisableVertexAttribArray(0);
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
      left_axis.horizontal = -((double)value / (double)range);
    }
  }
  else if(axis == 1){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      left_axis.vertical = 0.0;
    }
    else{
      left_axis.vertical = -((double)value / (double)range) ;
    }
  }
  else if(axis == 2){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      right_axis.horizontal = 0.0;
    }
    else{
      right_axis.horizontal = -((double)value / (double)range) * 0.1;
    }
  }
  else if(axis == 3){
    // linear scaling has to be changed, probably want x^2
    const int range = 32768;
    if(value > -5000 && value < 5000){
      right_axis.vertical = 0.0;
    }
    else{
      right_axis.vertical = -((double)value / (double)range) * 0.1;
    }
  }
}

void
main_scene_handle_key(SDL_Keycode *sym){
  switch(*sym){
  case SDLK_UP:
    camera.x -= ( GLdouble ) sin( camera.rotationY);
    camera.z -= ( GLdouble ) cos( camera.rotationY);
    break;
  case SDLK_DOWN:
    camera.x += ( GLdouble ) sin( camera.rotationY);
    camera.z += ( GLdouble ) cos( camera.rotationY);
    break;
  case SDLK_LEFT:
    camera.rotationY += ( GLdouble )0.1;
    break;
  case SDLK_RIGHT:
    camera.rotationY -= ( GLdouble )0.1;
    break;
  default:
    break;
  }
}
