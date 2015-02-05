#version 130
uniform vec3 lightColor;
uniform vec3 specularColor;
uniform vec3 ambient;

uniform float lightPow;
uniform float specPow;

uniform sampler2D texture;
uniform sampler2D normal_map;

in vec2 UV;
in vec3 EyeDirection;
in vec3 LightDirection;

out vec3 color;

void main(void) {
//    color = texture2D(texture, UV).xyz;
    vec3 texture_color = texture2D(texture, UV).rgb;

    vec3 ambient_part = ambient * texture_color;

    vec3 n = normalize(texture2D(normal_map, UV).rgb * 2.0 - 1.0);
    vec3 l = normalize(LightDirection);

    vec3 diffuse_part = clamp(dot(n, l), 0, 1) * texture_color * lightColor * lightPow;

    vec3 E = normalize(EyeDirection);
    vec3 R = reflect(-l, n);
    float cosAlpha = clamp((dot(E, R)), 0, 1);
    vec3 specular_part = specularColor * pow(cosAlpha, specPow);

    color = ambient_part + diffuse_part + specular_part;
}
