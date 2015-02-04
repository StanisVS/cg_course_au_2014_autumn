attribute vec4 qt_Vertex;
uniform mat4 mvp;

void main(void)
{
    gl_Position = mvp*qt_Vertex;
}
