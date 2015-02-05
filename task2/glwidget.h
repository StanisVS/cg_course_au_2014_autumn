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
#include <iostream>

using std::vector;
using std::cout;

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

public slots:
    void setLightColor(QColor color){
       cout<<"light setted \n"<<color.red();
    }
    void setAmbient (QColor color){
       cout<<"ambient setted \n"<<color.red();
    }
    void setSpecularColor (QColor color){
        cout<<"spec setted \n"<<color.red();
    }

    void setLightXangle (int x){
        cout<<x;
    }
    void setLightYangle (int y){
        cout<<y;
    }

    void setLightPower(double power){
       cout<<power;
    }
    void setSpecularPower(double power){
       cout<<power;
    }

private:
    struct Face {
        int vtx_idx[3];
        int uv_idx[3];
        int norm_idx[3];
    };

    void loadObjFile(QString filename) {
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
                norms.push_back(temp_normals[face.norm_idx[i] - 1]);
                tex_coords.push_back(textureUV[face.uv_idx[i] - 1]);
            }
        }
    }

    void loadModel(QString file_name) {
        triangles.clear();
        tex_coords.clear();
        norms.clear();
        tangents.clear();
        bitangents.clear();
        loadObjFile(":/" + file_name);
        computeTangentBasis();
    }

    void computeTangentBasis();

    void initTexParams();

    QGLShaderProgram* shader;

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;
    QMatrix3x3 matrix3x3;
    QMatrix4x4 modelview;
    QMatrix4x4 mvp;

    vector<QVector3D> triangles;
    GLuint trianglesAttrId = 0;
    vector<QVector2D> tex_coords;
    GLuint tex_coordsAttrId = 1;
    vector<QVector3D> norms;
    GLuint normsAttrId = 2;
    vector<QVector3D> tangents;
    GLuint tangentsAttrId = 3;
    vector<QVector3D> bitangents;
    GLuint bitangentsAttrId = 4;

    int xyModelAngle;
    int yzModelAngle;

    float multiplier;

    QVector3D lightDir;

    QVector3D lightColor;
    QVector3D ambient;
    QVector3D specularColor;

    float lightPow;
    float specPow;



    GLuint textureId;
    GLuint texture;
    GLuint normalTextureId;
    GLuint normalTexture;

    float distance;

    int mouseX;
    int mouseY;




};

#endif // GLWIDGET_H
