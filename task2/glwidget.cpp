#include "glwidget.h"

#include <QGLBuffer>
#include <QSlider>
#include <math.h>
#include <iostream>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    shader(new QGLShaderProgram()),
    xyModelAngle(0),
    yzModelAngle(0),
    multiplier(1.0),
    lightDir(1,0,0),
    lightColor(0,0,0),
    ambient(0.1f,0.1f,0.1f),
    specularColor(1.0,1.0,1.0),
    lightPow(1.0),
    specPow(1),
    distance(10.0)

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
    shader->bindAttributeLocation("vertex", trianglesAttrId);
    shader->bindAttributeLocation("texcoord",tex_coordsAttrId);
    shader->bindAttributeLocation("normals", normsAttrId);
    shader->bindAttributeLocation("tangents", tangentsAttrId);
    shader->bindAttributeLocation("bitangents", bitangentsAttrId);
    shader->link();
    shader->bind();
    shader->setUniformValue("texture",0);
    shader->setUniformValue("normal_map",1);


    glActiveTexture(GL_TEXTURE1);
    normalTexture = bindTexture(QPixmap(QString(":/normal.jpg")), GL_TEXTURE_2D);
    initTexParams();
    glActiveTexture(GL_TEXTURE0);
    texture = bindTexture(QPixmap(QString(":/texture.jpg")), GL_TEXTURE_2D);
    initTexParams();
}

void GLWidget::initTexParams()
{
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

    modelview = view*model;
    mvp = projection*modelview;
    matrix3x3 = modelview.normalMatrix();

    shader->setUniformValue("model", model);
    shader->setUniformValue("view", view);
    shader->setUniformValue("projection", projection);

    shader->setUniformValue("modelview", modelview);
    shader->setUniformValue("mvp", mvp);
    shader->setUniformValue("matrix3x3", matrix3x3);

    shader->setUniformValue("multiplier", multiplier);


    shader->setUniformValue("lightDir", lightDir);
    shader->setUniformValue("lightColor",lightColor);
    shader->setUniformValue("specularColor",specularColor);
    shader->setUniformValue("ambient",ambient);

    shader->setUniformValue("lightPow",lightPow);
    shader->setUniformValue("specPow",specPow);





    shader->enableAttributeArray(trianglesAttrId);//triangles
    shader->enableAttributeArray(tex_coordsAttrId);//tex_coords
    shader->enableAttributeArray(normsAttrId);//normals
    shader->enableAttributeArray(tangentsAttrId);//tangents
    shader->enableAttributeArray(bitangentsAttrId);//bitangets


    shader->setAttributeArray(trianglesAttrId, triangles.data());
    shader->setAttributeArray(tex_coordsAttrId, tex_coords.data());
    shader->setAttributeArray(normsAttrId,norms.data());
    shader->setAttributeArray(tangentsAttrId,tangents.data());
    shader->setAttributeArray(bitangentsAttrId,bitangents.data());



    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, triangles.size());
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
    xyModelAngle += event->x() - mouseX;
    yzModelAngle += event->y() - mouseY;
    model.setToIdentity();
    model.rotate(xyModelAngle, QVector3D(0, 1, 0));
    model.rotate(yzModelAngle, QVector3D(1, 0, 0));
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
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    if (!enabled) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    glBindTexture(GL_TEXTURE_2D, texture);
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

void GLWidget::computeTangentBasis()
{
    for (unsigned int i = 0; i < triangles.size(); i += 3 ) {
        QVector3D & v0 = triangles[i + 0];
        QVector3D & v1 = triangles[i + 1];
        QVector3D & v2 = triangles[i + 2];
        QVector2D & uv0 = tex_coords[i + 0];
        QVector2D & uv1 = tex_coords[i + 1];
        QVector2D & uv2 = tex_coords[i + 2];

        QVector3D deltaPos1 = v1 - v0;
        QVector3D deltaPos2 = v2 - v0;
        QVector2D deltaUV1 = uv1 - uv0;
        QVector2D deltaUV2 = uv2 - uv0;
        float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
        QVector3D tangent = (deltaPos1 * deltaUV2.y()   - deltaPos2 * deltaUV1.y()) * r;
        QVector3D bitangent = (deltaPos2 * deltaUV1.x()   - deltaPos1 * deltaUV2.x()) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }

}


