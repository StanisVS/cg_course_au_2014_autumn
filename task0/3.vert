attribute vec4 qt_Vertex;
varying vec2 pos;
uniform mat4 mvp;

void main(void)
{
    pos = qt_Vertex.xy;
    gl_Position = mvp * qt_Vertex;
}
