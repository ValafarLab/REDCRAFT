#include "analysissuite.h"
#include "ui_analysissuite.h"

AnalysisSuite::AnalysisSuite(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalysisSuite)
{
    ui->setupUi(this);
    scatter = new Scatter(this);
    dynamicprofile = this->findChild<DynamicProfile *>("dynamicprofile");
    tw = this->findChild<QTableWidget *>("twDataView");
    lw = this->findChild<QListWidget *>("lwInfoBox");
    sbStartResidue = this->findChild<QSpinBox *>("sbStartResidue");
    sbEndResidue = this->findChild<QSpinBox *>("sbEndResidue");
}

AnalysisSuite::~AnalysisSuite()
{
    delete ui;
    delete scatter;
}

void AnalysisSuite::openForAnalysis(QString outPath, QString name)
{
    this->outPath = outPath;
    QFile inputFile(outPath + "/" + name);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          std::istringstream stream(line.toStdString());
          std::vector<std::string> l;
          for (std::string s; stream >> s;) {
              l.push_back(s);
          }
          lines.push_back(l);
       }
    }
    tw->setRowCount((int) lines.size());
    tw->setColumnCount(lines.size() > 0 ? (int) lines[0].size() : 0);
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(lines[i][j]));
            tw->setItem(i, j, item);
        }
    }
    tw->resizeRowsToContents();
    tw->resizeColumnsToContents();
    //TODO: label headers different color
    QListWidgetItem *title = new QListWidgetItem();
    title->setFont(QFont(QApplication::font().family(), 16));
    title->setText(name);
    lw->addItem(title);
    lw->addItem("Number of rows: " + QString::number(lines.size()));
    lw->addItem("Number of columns: " + QString::number(lines.size() > 0 ? lines[0].size() : 0) );

    int y = name.split(".")[0].toInt();
    this->x = 2;
    this->y = y;
    sbEndResidue->setValue(y);
    dynamicprofile->loadOut(outPath, 2, y);
}

void AnalysisSuite::on_twDataView_cellDoubleClicked(int row, int column)
{
    scatter->loadPoints(lines[row]);
    scatter->show();
}

void AnalysisSuite::on_sbStartResidue_valueChanged(int arg1)
{
    this->x = arg1;
    dynamicprofile->loadOut(this->outPath, this->x, this->y);
}

void AnalysisSuite::on_sbEndResidue_valueChanged(int arg1)
{
    this->y = arg1;
    dynamicprofile->loadOut(this->outPath, this->x, this->y);
}
