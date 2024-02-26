#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QtDebug>

class Histogram : public QWidget
{
public:
    Histogram(QWidget *parent = nullptr);
    void loadResidues(bool, QList<QMap<QString, QList<double>>>, QList<QList<int>>, QList<QList<double>>);
    void resizeEvent(QResizeEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    QtCharts::QChart *chart;
    QtCharts::QChartView *chartView;
    QList<QMap<QString, QList<double>>> consolidated_residues_total;
    QList<QList<int>> residues_total;
    QList<QList<double>> media_total;
    bool consolidated = false;
};

#endif // HISTOGRAM_H
