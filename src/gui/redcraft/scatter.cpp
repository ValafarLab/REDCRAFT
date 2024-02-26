#include "scatter.h"

Scatter::Scatter(QWidget *parent) : QWidget(parent, Qt::Window)
{
    resize(800, 600);
    chart = new QtCharts::QChart();
    series = new QtCharts::QScatterSeries();
}

Scatter::~Scatter()
{
    delete chart;
    delete series;
}

void Scatter::loadPoints(std::vector<std::string> line)
{
    series->setName("Points");
    for (int i = 0; i<line.size()-2; i+=2) {
        series->append(std::stod(line[i]), std::stod(line[i+1]));
    }
    chart->addSeries(series);
    chart->setTitle("Scatterplot");
    this->setWindowTitle("Scatterplot");
    chart->setAnimationOptions(
    QtCharts::QChart::SeriesAnimations);
    chart->createDefaultAxes();
    chartView = new QtCharts::QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(this->size());
}

void Scatter::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    chartView->resize(this->size());
}
