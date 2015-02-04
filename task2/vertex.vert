#version 130

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float multiplier;

attribute vec4 vertex;
attribute vec2 texcoord;

varying vec2 v_texcoord;

void main(void) {
     gl_Position = projection * view * model * vertex;
     v_texcoord = multiplier * texcoord;
}
