#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>

class GLWidget : public QGLWidget
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
    void rotate ();
private:
    QTimer timer;
    QMatrix4x4 mvp;
    QMatrix4x4 rotation;
    QGLShaderProgram shaders[4];
    QGLShaderProgram *currentShader;
    QVector<QVector3D> vertices;

    float xoffset;
    float yoffset;
    float fscale;
    QPoint oldMousePos;

    void initShaders ();
};

#endif // GLWIDGET_H
