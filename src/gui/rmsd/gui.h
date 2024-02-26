#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include<QtCore>
#include<QtGui>
#include "FloatArray.h"
#include "PDBParser.h"
#include "Matrix.h"
#include "Vector.h"


namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gui(QWidget *parent = 0);
    void addToList(QString);
    void Runback();
    void addToList2(QString);
    void Runback2();

    QString setReportFlag();
    bool Evaluate(PDBParser, PDBParser,int,int,int,int);
    bool Evaluate(PDBParser,PDBParser);
    float calcDist(float x1, float x2, float y1, float y2, float z1, float z2);
    float calcRMSD(Matrix one, Matrix two, string reportFlag);
    void Angles(Matrix A, PDBParser D,string report);
    void Angles2(Matrix A, PDBParser D,string report);
    ~Gui();


public slots:



private slots:

    void on_actionOpen_triggered();



    void on_pushButton_2_clicked();



    void on_calcRMSD_clicked();

    void on_File1Show_clicked();

    void on_File2Show_clicked();

    void on_Atoms_currentIndexChanged(int index);

    void on_actionHelp_triggered();

    void on_actionAbout_triggered();

    void on_actionExport_Angles_triggered();

private:
    Ui::Gui *ui;
    QStringListModel *modelDisplay;
    QStringListModel *modelFile1;
    QStringListModel *modelFile2;
    QStringListModel *modelRMSD;
    QStringList lists;
    QStringList lists2;
    QStringList files;
    QFileDialog dialog;
};

#endif // GUI_H
