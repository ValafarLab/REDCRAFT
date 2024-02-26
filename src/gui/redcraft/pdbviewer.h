#ifndef PDB_VIEWER_H
#define PDB_VIEWER_H

#include <QOpenGLWidget>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <vector>
#include <tuple>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>

//#include <GL/glu.h>
//#include <GL/gl.h>

class PDBViewer : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    PDBViewer(QWidget *parent = nullptr);

    void readObj(QString path);


protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    // Members for storing OBJ triangles
    std::vector<std::tuple<float, float, float>> v;
    std::vector<std::tuple<float,float>> vt;
    std::vector<std::tuple<float, float, float>> n;
};

#endif // PDB_VIEWER_H
