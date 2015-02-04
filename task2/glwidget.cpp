#include "glwidget.h"

#include <QGLBuffer>
#include <QSlider>
#include <math.h>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    shader(new QGLShaderProgram()),
    distance(10.0),
    hAngle(0),
    vAngle(0),
    multiplier(1.0)
{
    projection.perspective(30.0f, 1.1f, 0.1f, 100.f);
}

void GLWidget::initializeGL()
{
    loadModel("quad.obj");

    initializeGLFunctions();

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    shader->addShaderFromSourceFile(QGLShader::Vertex,":/vertex.vert");
    shader->addShaderFromSourceFile(QGLShader::Fragment,":/fragment.frag");
    shader->bindAttributeLocation("vertex", 0);
    shader->bindAttributeLocation("texcoord", 1);
    shader->link();
    shader->bind();
    shader->setUniformValue("texture", 0);

    texture = bindTexture(QPixmap(QString(":/brickwork-texture.jpg")), GL_TEXTURE_2D);
    normalTexture = bindTexture(QPixmap(QString(":/brickwork_nomal-map.jpg")), GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);
}

void GLWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view.setToIdentity();
    view.lookAt(QVector3D(0.0f, 0.0f, -distance), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));

    shader->setUniformValue("model", model);
    shader->setUniformValue("view", view);
    shader->setUniformValue("projection", projection);

    shader->setUniformValue("multiplier", multiplier);

    shader->enableAttributeArray(0);
    shader->enableAttributeArray(1);
    shader->setAttributeArray(0, triangles.data());
    shader->setAttributeArray(1, tex_coords.data());


    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, triangles.size());
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
    hAngle += event->x() - mouseX;
    vAngle += event->y() - mouseY;
    model.setToIdentity();
    model.rotate(hAngle, QVector3D(0, 1, 0));
    model.rotate(vAngle, QVector3D(1, 0, 0));
    mouseX = event->x();
    mouseY = event->y();
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent* event) {
    mouseX = event->x();
    mouseY = event->y();
}

void GLWidget::wheelEvent(QWheelEvent* event) {
    distance -= event->delta() / 120;
    distance = std::max(distance, 0.0f);
    updateGL();
}

void GLWidget::switchModel(int index)
{
    switch (index) {
    case 1:
        loadModel("sphere.obj");
        break;
    case 2:
        loadModel("cylinder.obj");
        break;
    case 0:
    default:
        loadModel("quad.obj");
    }
    updateGL();
}

void GLWidget::enableFiltering(bool enabled)
{
    if (!enabled) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    updateGL();
}

void GLWidget::setMultiplier(double multiplier)
{
    this->multiplier = multiplier;
    updateGL();
}
