#ifndef ANALYSISSUITE_H
#define ANALYSISSUITE_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSpinBox>

#include <QFile>
#include <QTextStream>
#include <sstream>

#include "scatter.h"
#include "dynamicprofile.h"

namespace Ui {
class AnalysisSuite;
}

class AnalysisSuite : public QMainWindow
{
    Q_OBJECT

public:
    explicit AnalysisSuite(QWidget *parent = nullptr);
    ~AnalysisSuite();
    void openForAnalysis(QString outPath, QString name);
    void analyzeLine(int line);
//    void resizeEvent(QResizeEvent *event);

private slots:
    void on_twDataView_cellDoubleClicked(int row, int column);

    void on_sbStartResidue_valueChanged(int arg1);

    void on_sbEndResidue_valueChanged(int arg1);

private:
    Ui::AnalysisSuite *ui;
    std::vector<std::vector<std::string>> lines;
    QTableWidget *tw;
    QListWidget *lw;
    QSpinBox *sbStartResidue;
    QSpinBox *sbEndResidue;
    Scatter *scatter;
    DynamicProfile *dynamicprofile;
    QString outPath;
    int x, y;
};

#endif // ANALYSISSUITE_H
