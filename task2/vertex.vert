#version 130

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 mvp;
uniform mat4 modelview;
uniform mat3 matrix3x3;
uniform vec3 lightDir;


uniform float multiplier;

attribute vec4 vertex;
attribute vec2 texcoord;
attribute vec3 normals;
attribute vec3 tangents;
attribute vec3 bitangents;



out vec2 UV;
out vec3 EyeDirection;
out vec3 LightDirection;

void main(void) {
    gl_Position = projection*view*model * vertex;

    vec3 vert_normal_cameraspace = matrix3x3 * normalize(normals);
    vec3 vert_tangent_cameraspace = matrix3x3 * normalize(tangents);
    vec3 vert_bitangent_cameraspace = matrix3x3 * normalize(bitangents);
    mat3 TBN = transpose(mat3(vert_tangent_cameraspace, vert_bitangent_cameraspace, vert_normal_cameraspace));

    vec4 ed = modelview*vertex;
    LightDirection = TBN * (-lightDir);
    EyeDirection = TBN * (-ed.xyz);
    UV = multiplier * texcoord;
}
