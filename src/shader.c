/*
 * William Emerison Six
 *
 * Copyright 2016-2017 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */

#include "common.h"

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <unistd.h>
#endif
// wrapper to handle ignore errors, and to
// retry reading from the stream
size_t xfread(void *ptr,
              size_t size,
              size_t nmemb,
              FILE *stream)
{
  size_t result = 0;
 attemptToRead:
  result += fread(ptr, size, nmemb, stream);
  if(result)
    return result;
  if(feof(stream))
    return result;
  goto attemptToRead;
}



GLuint
compile_shader(GLenum shaderType, const char* const path)//GLchar ** shader_text)
{

  struct stat buf;
  if (0 != stat(path,
                &buf))
    {
      // TODO check errno
    }

  // no need to do size+1 because glshadersource takes len as an arg
  char * shader_text = (char*) malloc(buf.st_size);

  FILE *file  = fopen(path, "r");
  size_t read_size = xfread(shader_text,
                            sizeof(char),
                            buf.st_size,
                            file);
  fclose(file);




  GLuint shaderID = glCreateShader(shaderType);
  GL_DEBUG_ASSERT();

  if(shaderType == GL_VERTEX_SHADER){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "Compiling Vertex shader : %s\n",
                   shader_text);
  }
  else if (shaderType == GL_FRAGMENT_SHADER) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "Compiling Fragment shader : %s\n",
                   shader_text);
  }

  {
    GLint bufferSize = buf.st_size;
    const char ** the_text = (const char **)&shader_text;
    glShaderSource(shaderID,
                   1,
                   the_text,
                   &bufferSize);
  }
  glCompileShader(shaderID);

  GLint Result = GL_FALSE;
  int32_t InfoLogLength;

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

  free(shader_text);

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
  int32_t InfoLogLength;

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
