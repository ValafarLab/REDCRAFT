#include "dynamicprofile.h"

DynamicProfile::DynamicProfile(QWidget *parent) : QWidget(parent)
{

//    int nWidth = 400;
//    int nHeight = 300;
//    if (parent != nullptr)
//        resize(parent->size()/(1.5));
//    else
//        resize(nWidth, nHeight);
    resize(800, 600);
    chart = new QtCharts::QChart();
    auto *layout = new QVBoxLayout(this);
    chartView = new QtCharts::QChartView(chart);
    exportAsCSV = new QPushButton("Export As CSV...");    layout->addWidget(exportAsCSV);
    layout->addWidget(chartView);
    connect(exportAsCSV, &QPushButton::clicked, this, [&, this]() {
        if (this->series != nullptr && this->last_series != nullptr) {
            QString filename = QFileDialog::getSaveFileName(this, "Export Dynamic Profile As Csv..", "dynamicprofile.csv", "CSV files (.csv)", &path, 0); // getting the filename (full path)
            QFile data(filename);
            if(data.open(QFile::WriteOnly |QFile::Truncate))
            {
                QTextStream output(&data);
                output << "Best series,,Current series\n";

                for (int i = 0; i < series->points().size(); i++) {
                    auto series_point = series->at(i);
                    auto series_x = series_point.x();
                    auto series_y = series_point.y();

                    auto last_point = last_series->at(i);
                    auto last_x = last_point.x();
                    auto last_y = last_point.y();
                    output << series_x << "," << series_y << ","
                           << last_x << "," << last_y << "\n";
                }
                data.close();
            }
        }
    });
//    chartView = new QtCharts::QChartView(chart);
}

void DynamicProfile::loadOut(QString path, int x, int y)
{
    this->path = path;
    this->x = x;
    this->y = y;
    int n = y;
    // First, remove the chart (since this function is potentially called multiple times)
    QtCharts::QChart *old_chart = nullptr;
    if (chartView->chart()) old_chart = chartView->chart();
    QtCharts::QChart *chart = new QtCharts::QChart();
    series = new QtCharts::QLineSeries();
    series->setName("Best profile");
    QColor red;
    red.setRed(255);
    series->setColor(red);
    last_series = new QtCharts::QLineSeries();
    last_series->setName("Last profile");

    // To calculate the dynamic profile, we will iterate through files 2.out .. n.out, find the maximum value of the last column, and plot it against 2 .. n
    double min = INT_MAX;
    double max = -INT_MAX+1;
    for (int i = x; i <= n; i++) {
        QString true_path = QString("/%1.out").arg(i);
        QFile inputFile(path + true_path);
        double min_val = 10000000;
        std::vector<std::string> l;
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           l.clear();
           while (!in.atEnd())
           {
              QString line = in.readLine();
              std::istringstream stream(line.toStdString());
              for (std::string s; stream >> s;) {
                  l.push_back(s);
              }
              double last_val = std::stod(l.back());
              if (last_val < min_val) min_val = last_val;
              if (last_val < min) min = last_val;
              if (last_val > max) max = last_val;
           }
        } else {
            continue;
//            throw std::runtime_error(("Cannot construct dynamic profile as " + true_path + " is missing.").toStdString());
        }
        double last_val = std::stod(l.back());
        inputFile.close();
        series->append(i, min_val);
        last_series->append(i, last_val);

    }
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setRange(min,max);
    chart->addAxis(axisY, Qt::AlignLeft);

    QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis();
    axisX->setTickCount(n-x+1);
    axisX->setRange(x, n);
    axisX->setLabelFormat("%d");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->createDefaultAxes();

    chart->addSeries(series);
    chart->addSeries(last_series);

    series->attachAxis(axisX);
    last_series->attachAxis(axisX);
    series->attachAxis(axisY);
    last_series->attachAxis(axisY);

    chart->setTitle("Dynamic Profile for: " + QString::number(y));
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(this->size());

    chartView->setChart(chart);
    delete old_chart;
}

void DynamicProfile::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    chartView->resize(this->size());
}

void DynamicProfile::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        if (!path.isEmpty()) {
            DynamicProfile *out = new DynamicProfile();
            out->loadOut(path, x, y);
            out->show();
        } else {
            qDebug() << "Unable to open large profile!\n";
        }
    }
}


