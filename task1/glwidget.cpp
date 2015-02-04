#include "glwidget.h"
#include <QSlider>
#include <QGLBuffer>
#include <math.h>
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent), timer(this), time(0.0)
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    projection.perspective(30.0f, 4.0f / 3.0f, 0.1f, 100.f);
}

void GLWidget::initializeGL()
{
    glClearColor(1,1,1,1);
    loadObjFile(":/model.obj");
    initShaders();
    initializeGLFunctions();
    initBuffers();
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    timer.start(10);
}



void GLWidget::initShaders(){
    shaders[0].addShaderFromSourceFile(QGLShader::Vertex,":/1.vert");
    shaders[0].addShaderFromSourceFile(QGLShader::Fragment,":/1.frag");
    shaders[1].addShaderFromSourceFile(QGLShader::Vertex,":/1.vert");
    shaders[1].addShaderFromSourceFile(QGLShader::Fragment,":/1.frag");
    shaders[2].addShaderFromSourceFile(QGLShader::Vertex,":/2.vert");
    shaders[2].addShaderFromSourceFile(QGLShader::Fragment,":/2.frag");
    for (int i=0;i<3;++i){
        shaders[i].link();
    }

    QSlider* kSlider = this->parentWidget()->findChild<QSlider*>("k");
    QSlider* vSlider = this->parentWidget()->findChild<QSlider*>("v");
    kSlider->setEnabled(false);
    vSlider->setEnabled(false);

    max_distance = 0.0;
    for(std::vector<QVector3D>::iterator it = triangles.begin(); it != triangles.end(); ++it){
        center += (*it);
    }
    center /= triangles.size();

    for(std::vector<QVector3D>::iterator it = triangles.begin(); it != triangles.end(); ++it){
        max_distance = std::max(max_distance, ((*it) - center).length());
    }

    currentShader = &shaders[0];
    currentShader->bind();


}

void GLWidget::initBuffers()
{
    glGenBuffers(2, buffersIDs);

    glBindBuffer(GL_ARRAY_BUFFER, buffersIDs[0]);
    glBufferData(GL_ARRAY_BUFFER, triangles.size() * sizeof(float) * 3, triangles.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffersIDs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, normals.size() * sizeof(float) * 3, normals.data(), GL_STATIC_DRAW);

}

void GLWidget::loadObjFile(QString filename) {
    QFile file(filename);
    std::vector<QVector3D> temp_triangles;
    std::vector<QVector3D> temp_normals;
    std::vector<QVector2D> textureUV;
    std::vector<Face> faces;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Cannot open " << filename << endl;
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList elements = line.split(QRegExp("\\s+"));
        if (elements[0] == "v") {
            temp_triangles.push_back(QVector3D(elements[1].toFloat(), elements[2].toFloat(), elements[3].toFloat()));
        }else if(elements[0] == "vn"){
            temp_normals.push_back(QVector3D(elements[1].toFloat(), elements[2].toFloat(), elements[3].toFloat()));
        } else if(elements[0] == "vt"){
            textureUV.push_back(QVector2D(elements[1].toFloat(), elements[2].toFloat()));
        }  else if (elements[0] == "f") {
            Face face;
            for(int i = 0 ; i < 3; ++i){
                QStringList numbers = elements[i+1].split("/");
                face.vtx_idx[i] = numbers[0].toInt();
                face.uv_idx[i] = numbers[1].toInt();
                face.norm_idx[i] = numbers[2].toInt();
            }
            faces.push_back(face);
        }
        else if (line[0] == '#') { /* ignoring this line */ }
        else { /* ignoring this line */ }
    }

    for (auto face : faces) {
        for (int i = 0; i < 3; ++i) {
            triangles.push_back(temp_triangles[face.vtx_idx[i] - 1]);
            normals.push_back(temp_normals[face.norm_idx[i] - 1]);
        }
    }
}



void GLWidget::paintGL()
{
    glClearColor(0, 0, 0, 1.0f);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view.setToIdentity();
    view.lookAt(QVector3D(0.0f, -distance * std::sin(theta * M_2_PI / 90), -distance * std::cos(theta * M_2_PI / 90)), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    currentShader->setUniformValue("mvp", projection * view * model);
    currentShader->setUniformValue("model", model);

    glBindBuffer(GL_ARRAY_BUFFER, buffersIDs[0]);
    currentShader->setAttributeBuffer( "vertex", GL_FLOAT, 0, 3 );
    currentShader->enableAttributeArray( "vertex" );

    glBindBuffer(GL_ARRAY_BUFFER, buffersIDs[1]);
    currentShader->setAttributeBuffer( "norm", GL_FLOAT, 0, 3 );
    currentShader->enableAttributeArray( "norm" );

    currentShader->setUniformValue("color", QVector3D(1,1,1));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, triangles.size());

    if(skelet){
        currentShader->setUniformValue("color", QVector3D(0.5,0.5,0.5));
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-2, -2);
        glDrawArrays(GL_TRIANGLES, 0, triangles.size());
    }
    currentShader->disableAttributeArray( "vertex" );
    swapBuffers();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::changeShader (int i){
    currentShader = &shaders[i];
    QSlider* kSlider = this->parentWidget()->findChild<QSlider*>("k");
    QSlider* vSlider = this->parentWidget()->findChild<QSlider*>("v");
    currentShader->bind();
    if(i<2){
        kSlider->setEnabled(false);
        vSlider->setEnabled(false);
        currentShader->setUniformValue("colored",i==1);
    }else{
        currentShader->setUniformValue("v", (float) 0);
        currentShader->setUniformValue("center", center);
        currentShader->setUniformValue("k", (float) M_2_PI / max_distance);
        currentShader->setUniformValue("time", time);
        kSlider->setEnabled(true);
        vSlider->setEnabled(true);
        timer.start(30);
    }
    updateGL();
}

void GLWidget::changeK(int i)
{
    if(currentShader==&shaders[2]){
        currentShader->setUniformValue("k", (float) (2 * M_PI * i / max_distance));
    }
}

void GLWidget::changeV(int i)
{
    if(currentShader==&shaders[2]){
        currentShader->setUniformValue("v", (float) (2 * M_PI * i));
    }
}

void GLWidget::mousePressEvent(QMouseEvent * event){
    oldMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent * event){
    if(!(event->buttons() & Qt::LeftButton)) return;
    model.rotate( -(event->y() - oldMousePos.y()), 1, 0);
    model.rotate( -(event->x() - oldMousePos.x()), 0, 1);
    oldMousePos = QPoint(event->x(),event->y());
}

void GLWidget::wheelEvent(QWheelEvent* event) {
    distance -= event->delta() / 120;
    distance = std::max(distance, 0.0f);
}

void GLWidget::updateTime()
{
    time += ((float) timer.interval()) / 1000.0;
    if(currentShader==&shaders[2]){
        currentShader->setUniformValue("time", time);
    }
    updateGL();
}

