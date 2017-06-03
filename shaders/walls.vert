#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vUV;

out VS_OUT {
  vec2 uv;
} vs_out;

uniform mat4 mvpMatrix;

void
main(){
   vs_out.uv = vUV;
   gl_Position = mvpMatrix * vec4(vPosition,1.0);
}
