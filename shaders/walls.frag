#version 330 core

in VS_OUT {
  vec2 uv;
} fs_in;
uniform sampler2D wallTexture;

out vec4 color;

void main(){
  color = texture( wallTexture, fs_in.uv * 20 );
}
