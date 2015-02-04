#version 130

uniform mat4 mvp;
uniform mat4 model;
uniform vec3 color;
in vec3 vertex;
in vec3 norm;

out vec4 pass_norm;
out vec4 pass_color;

uniform bool colored;

void main(void)
{
     gl_Position = mvp * vec4(vertex, 1.0);
     pass_norm = model * vec4(norm, 1.0);
     if(colored){
        pass_color = vec4(pass_norm.y, pass_norm.y, pass_norm.y, 1);
     } else{
        pass_color = vec4(color, 1.0);
     }
}
