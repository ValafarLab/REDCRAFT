#include "pdbviewer.h"

PDBViewer::PDBViewer(QWidget *parent)
    : QOpenGLWidget(parent) {}

void PDBViewer::readObj(QString p)
{
    QFile inputFile(p);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       std::vector<int> vInds, vtInds, nInds;
       std::vector<std::tuple<float, float, float>> temp_v;
       std::vector<std::tuple<float, float>> temp_vt;
       std::vector<std::tuple<float, float, float>> temp_n;
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          std::istringstream stream(line.toStdString());
          std::vector<std::string> l;
          for (std::string s; stream >> s;) {
              l.push_back(s);
          }
          if (l.size() > 0) {
              if (l[0] == "v") {
                  std::tuple<float,float,float> vertex(std::stof(l[1]), std::stof(l[2]), std::stof(l[3]));
                  temp_v.push_back(vertex);
              }
              if (l[0] == "vt") {
                  std::tuple<float,float> vertex(std::stof(l[1]), std::stof(l[2]));
                  temp_vt.push_back(vertex);
              }
              if (l[0] == "n") {
                  std::tuple<float,float,float> vertex(std::stof(l[1]), std::stof(l[2]), std::stof(l[3]));
                  temp_n.push_back(vertex);
              }
              if (l[0] == "f") {
                  int vIndices[3], vtIndices[3], nIndices[3];
                  for (int i = 0; i < 3; i++) {
                      int matches = sscanf(l[i+1].c_str(), "%d/%d/%d", &vIndices[i], &vtIndices[i], &nIndices[i]);
                      if (matches < 3) {
                          // Sometimes obj files are weird and omit vt
                          sscanf(l[i+1].c_str(), "%d//%d", &vIndices[i], &nIndices[i]);
                          vtIndices[i] = 0;
                      }
                      vInds.push_back(vIndices[i]);
                      vtInds.push_back(vtIndices[i]);
                      nInds.push_back(nIndices[i]);
                  }

              }
          }

          qDebug() << line << "\n";
       }
       inputFile.close();
       for (int i = 0; i < vInds.size(); i++) {
           int vIndex = vInds[i];
           int vtIndex = vtInds[i];
           int nIndex = nInds[i];
           v.push_back(temp_v[vIndex-1]);
           if (vtIndex != 0 && temp_vt.size() > 0)
             vt.push_back(temp_vt[vtIndex-1]);
           else {
               std::tuple<float,float> t(0.0f, 0.0f);
               vt.push_back(t);
           }
           if (temp_n.size() > 0) {
           n.push_back(temp_n[nIndex-1]);
           }
           else {
               std::tuple<float,float,float> t(0.0f, 0.0f, 0.0f);
               n.push_back(t);
           }
       }
    }
}

void PDBViewer::initializeGL()
{
//    readObj("/home/jrachele/Downloads/1A1Z.obj");
    initializeOpenGLFunctions();
    glClearColor(0,0,0,1);
    glDepthRange(-3, 3);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void PDBViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < v.size(); i++) {
        auto t = v[i];
        glColor3f(std::get<0>(t), std::get<1>(t), std::get<2>(t));
        glVertex3f(std::get<0>(t), std::get<1>(t), std::get<2>(t));
    }
    glEnd();
}

void PDBViewer::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    gluLookAt(0,0,5,0,0,0,0,1,0);
}
