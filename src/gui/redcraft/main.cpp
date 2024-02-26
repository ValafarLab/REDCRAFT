#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString path = "";
    if (argc > 1) {
        // We were likely passed a path
        path = argv[1];
    }
    MainWindow w(nullptr, path);
    w.show();

    // set stylesheet on unix
#ifndef _WIN32
    QFile file(":/light.qss");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    a.setStyleSheet(stream.readAll());
#endif

    return a.exec();
}
