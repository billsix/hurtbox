/*
 * William Emerison Six
 *
 * Copyright 2016 - William Emerison Six
 * All rights reserved
 * Distributed under LGPL 2.1 or Apache 2.0
 */


GLuint
compile_shader(GLenum shaderType, const GLchar ** shader_text);

GLuint
link_shaders(GLuint vertexShader, GLuint fragmentShader);

