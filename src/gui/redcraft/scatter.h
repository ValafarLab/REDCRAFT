#ifndef SCATTER_H
#define SCATTER_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>

class Scatter : public QWidget
{
    Q_OBJECT
public:
    explicit Scatter(QWidget *parent = nullptr);
    ~Scatter();
    void loadPoints(std::vector<std::string> line);
    void resizeEvent(QResizeEvent *event);
private:
    QtCharts::QChart *chart;
    QtCharts::QScatterSeries *series;
    QtCharts::QChartView *chartView;

signals:

public slots:
};

#endif // SCATTER_H
