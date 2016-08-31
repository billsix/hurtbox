/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include "common.h"

GLuint
compile_shader(GLenum shaderType, const GLchar ** shader_text)
{
  GLuint shaderID = glCreateShader(shaderType);
  GL_DEBUG_ASSERT();

  if(shaderType == GL_VERTEX_SHADER){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "Compiling Vertex shader : %s\n",
                   *shader_text);
  }
  else if (shaderType == GL_FRAGMENT_SHADER) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "Compiling Fragment shader : %s\n",
                   *shader_text);
  }
  
  glShaderSource(shaderID, 1, shader_text, NULL);

  
  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Check Shader
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
  GL_DEBUG_ASSERT();
  glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  GL_DEBUG_ASSERT();
  if (InfoLogLength > 0) {
    char *log = (char*)malloc(InfoLogLength * sizeof(char));
    glGetShaderInfoLog(shaderID,
                       InfoLogLength,
                       NULL,
                       log);
    GL_DEBUG_ASSERT();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "Shader info %s\n",
                   log);
    free(log);
  }

  return shaderID;
}


GLuint
link_shaders(GLuint vertexShader, GLuint fragmentShader)
{
  // Link the program
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                 SDL_LOG_PRIORITY_INFO,
                 "Linking program\n");

  GLuint programID = glCreateProgram();
  glAttachShader(programID, vertexShader);
  GL_DEBUG_ASSERT();
  glAttachShader(programID, fragmentShader);
  GL_DEBUG_ASSERT();
  glLinkProgram(programID);
  GL_DEBUG_ASSERT();


  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Check the program
  glGetProgramiv(programID, GL_LINK_STATUS, &Result);
  GL_DEBUG_ASSERT();
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  GL_DEBUG_ASSERT();
  if (InfoLogLength > 0) {
    char *log = (char*)malloc(InfoLogLength * sizeof(char));
    glGetProgramInfoLog(programID,
			InfoLogLength,
			NULL,
			log);
    GL_DEBUG_ASSERT();
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "Linking info %s\n",
                   log);
    free(log);

  }


  glDetachShader(programID, vertexShader);
  GL_DEBUG_ASSERT();
  glDetachShader(programID, fragmentShader);
  GL_DEBUG_ASSERT();
  return programID;

}
