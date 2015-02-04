#version 130

in vec4 pass_pos;
in vec4 pass_norm;
in vec4 pass_color;
out vec4 out_Color;

uniform vec3 center;
uniform float time;
uniform float k;
uniform float v;

void main(void) {
      float r = abs(sin(v * time + k * length(pass_pos - vec4(center, 1))));
      float g = abs(sin(1.1 * v * time + k * length(pass_pos - vec4(center, 1))));
      float b = abs(sin(0.9 * v * time + k * length(pass_pos - vec4(center, 1))));
      out_Color = vec4(r, g, b, 1);
}
