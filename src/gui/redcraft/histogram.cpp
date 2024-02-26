#include "histogram.h"

Histogram::Histogram(QWidget *parent) : QWidget(parent)
{
    resize(800,600);
    chart = new QtCharts::QChart();
    chartView = new QtCharts::QChartView(chart, this);
}

void Histogram::loadResidues(bool consolidated, QList<QMap<QString, QList<double>>> consolidated_residues_total, QList<QList<int>> residues_total, QList<QList<double>> media_total)
{
    // Consolidated residues will form a stacked histogram
    // Non-consolidated will form a line graph sans 999
    this->consolidated_residues_total = consolidated_residues_total;
    this->residues_total = residues_total;
    this->media_total = media_total;
    this->consolidated = consolidated;

    // First, remove the chart (since this function is potentially called multiple times)
    QtCharts::QChart *old_chart = nullptr;
    if (chartView->chart()) old_chart = chartView->chart();
    QtCharts::QChart *chart = new QtCharts::QChart();

    if (consolidated) {
        // for now, we will only base it off of the first alignment media
        if (consolidated_residues_total.isEmpty()) {
            qDebug() << "No info for consolidated residues";
            return;
        }
        QMap<QString, QList<double>> consolidated_residues = consolidated_residues_total[0];
        // Iterate through the residues and create corresponding barsets
        QList<QString> keys = consolidated_residues.keys();
        QtCharts::QBarSet *normalResidues = new QtCharts::QBarSet("Normal");
        QtCharts::QBarSet *unknownResidues = new QtCharts::QBarSet("999");
        for (QString key : keys) {
            QList<double> vals = consolidated_residues[key];
            int unknown_count = 0;
            for (double val : vals) {
                if (val == 999) {
                    unknown_count++;
                }
            }
            *normalResidues << (vals.length() - unknown_count);
            *unknownResidues << unknown_count;
        }
        QtCharts::QStackedBarSeries *series = new QtCharts::QStackedBarSeries();
        series->append(normalResidues);
        series->append(unknownResidues);
        chart->addSeries(series);
        chart->setTitle("RDC unknown distribution");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
        QtCharts::QBarCategoryAxis *axisX = new QtCharts::QBarCategoryAxis();
        axisX->append(keys);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
        chartView->setRenderHint(QPainter::Antialiasing);
    } else {
        // We must iterate through each alignment media and produce a barset for each
        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        for (int j = 0; j < residues_total.length(); j++) {
            QList<int> residues = residues_total[j];
            QList<double> media = media_total[j];
            QtCharts::QBarSet *media_set = new QtCharts::QBarSet("M" + QString::number((j+1)));
            for (int i = 0; i < media.length(); i++) {
                *media_set << (6 - media[i]);
            }
            series->append(media_set);
        }
            chart->addSeries(series);
            chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
            QtCharts::QBarCategoryAxis *axisX = new QtCharts::QBarCategoryAxis();
            QStringList categories;
            for (auto res : residues_total[0]) {
                categories << QString::number(res);
            }
            axisX->append(categories);
            chart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);
            QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
            axisY->setRange(0, 6);
            axisY->setLabelFormat("%d");
            axisY->setTickCount(7);
            axisX->setGridLineVisible(false);
            axisY->setGridLineVisible(false);
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
            chart->setTitle("Number of valid bonds per residue");
            chartView->setRenderHint(QPainter::Antialiasing);

    }

    chartView->setChart(chart);
    delete old_chart;

}

void Histogram::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    chartView->resize(this->size());
}

void Histogram::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
            Histogram *out = new Histogram();
            out->loadResidues(consolidated, consolidated_residues_total, residues_total, media_total);
            out->show();
    }
}
