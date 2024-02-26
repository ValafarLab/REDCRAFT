#ifndef DYNAMICPROFILE_H
#define DYNAMICPROFILE_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <sstream>

class DynamicProfile : public QWidget
{
public:
    DynamicProfile(QWidget *parent = nullptr);
    void loadOut(QString path, int x, int y);
    void resizeEvent(QResizeEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    QtCharts::QChart *chart;
    QtCharts::QChartView *chartView;
    QString path;
    QPushButton *exportAsCSV;
    int x, y;
    QtCharts::QLineSeries *series;
    QtCharts::QLineSeries *last_series;
};

#endif // DYNAMICPROFILE_H
