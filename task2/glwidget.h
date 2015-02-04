#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShader>
#include <QFileInfo>
#include <QTimer>
#include <QGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>

#include <vector>

using std::vector;

class GLWidget : public QGLWidget, public QGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

protected slots:
    void switchModel(int index);
    void enableFiltering(bool enabled);
    void setMultiplier(double multiplier);

private:
    struct Face {
        int vtx_idx[3];
        int uv_idx[3];
        int norm_idx[3];
    };

    void loadObjFile(QString filename) {
        QFile file(filename);
        std::vector<QVector3D> temp_triangles;
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
//                temp_normals.push_back(QVector3D(elements[1].toFloat(), elements[2].toFloat(), elements[3].toFloat()));
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
                tex_coords.push_back(textureUV[face.uv_idx[i] - 1]);
            }
        }
    }

    void loadModel(QString file_name) {
        triangles.clear();
        tex_coords.clear();
        loadObjFile(":/" + file_name);
    }

    QGLShaderProgram* shader;

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;

    vector<QVector3D> triangles;
    vector<QVector2D> tex_coords;

    GLuint texture;
    GLuint normalTexture;

    float distance;

    int mouseX;
    int mouseY;

    int hAngle;
    int vAngle;

    float multiplier;
};

#endif // GLWIDGET_H
