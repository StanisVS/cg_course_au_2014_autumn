#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent),xoffset(0.0),yoffset(0.0),fscale(1.0)
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(rotate()));
}

void GLWidget::initializeGL()
{
    glClearColor(1,1,1,1);
    vertices << QVector3D(-0.866f,-0.5f,0.0f) << QVector3D(0.866f,-0.5f,0.0f)
             << QVector3D(0.0f,1.0f,0.0f);
    initShaders();
    timer.start(10);
}

void GLWidget::rotate (){
    rotation.rotate(2,QVector3D(0,0,1));
    updateGL();
}

void GLWidget::initShaders(){
    shaders[0].addShaderFromSourceFile(QGLShader::Vertex,":/1.vert");
    shaders[0].addShaderFromSourceFile(QGLShader::Fragment,":/1.frag");
    shaders[1].addShaderFromSourceFile(QGLShader::Vertex,":/2.vert");
    shaders[1].addShaderFromSourceFile(QGLShader::Fragment,":/2.frag");
    shaders[2].addShaderFromSourceFile(QGLShader::Vertex,":/3.vert");
    shaders[2].addShaderFromSourceFile(QGLShader::Fragment,":/3.frag");
    shaders[3].addShaderFromSourceFile(QGLShader::Fragment,":/4.frag");
    for (int i=0;i<3;++i){
        shaders[i].link();
    }
    currentShader = &shaders[0];
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    currentShader->bind();

    if(currentShader!=&shaders[3]){
        currentShader->setUniformValue("mvp",rotation);
        currentShader->setAttributeArray("qt_Vertex",vertices.constData());
        currentShader->enableAttributeArray("qt_Vertex");
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        currentShader->disableAttributeArray("qt_Vertex");

    }else{
        currentShader->setUniformValue("texture", 0);
        currentShader->setUniformValue("fscale", fscale);
        currentShader->setUniformValue("cx", xoffset);
        currentShader->setUniformValue("cy", yoffset);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(-1, -1);
        glTexCoord2f(1, 0);
        glVertex2f(1, -1);
        glTexCoord2f(1, 1);
        glVertex2f(1, 1);
        glTexCoord2f(0, 1);
        glVertex2f(-1, 1);
        glEnd();
    }
     currentShader->release();

}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::changeShader (int i){
    currentShader = &shaders[i];
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent* event) {
    oldMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
    if(!(event->buttons() & Qt::LeftButton)) return;
    xoffset -= 2.0 * (event->x() - oldMousePos.x()) / float(width());
    yoffset -= 2.0 * (event->y() - oldMousePos.y()) / float(height());
    oldMousePos = event->pos();
    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent* event) {
    float sf = event->delta() > 0 ? 1.1 : 0.9;
    fscale *= sf;
    updateGL();
}

