#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>

class GLWidget : public QGLWidget, public QGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL();
    void paintGL();
    void resizeGL(int w , int h );

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
public slots:
    void changeShader (int i);
    void changeK (int i);
    void changeV (int i);
    void setSkelet (bool s){
        skelet=s;
        updateGL();
    }
    void updateTime();
private:
    QTimer timer;
    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;
    QGLShaderProgram shaders[3];
    QGLShaderProgram *currentShader;


    QPoint oldMousePos;

    void initShaders ();
    void initBuffers();

    struct Face {
        int vtx_idx[3];
        int uv_idx[3];
        int norm_idx[3];
    };
    void loadObjFile(QString file);

    GLuint buffersIDs[2];
    std::vector <QVector3D >  triangles;
    std::vector < QVector2D >  uvs;
    std::vector < QVector3D >  normals;

    bool skelet = false;

    float distance = 25.0f;
    float theta = 0;

    float time =0;
    QVector3D center;
    float max_distance;

};

#endif // GLWIDGET_H
