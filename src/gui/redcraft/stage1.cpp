#include "mainwindow.h"

void MainWindow::updateStage1Text() {
    QTextStream rsyncStdoutStream(process->readAllStandardOutput());
        while (true)
        {
            QString line = rsyncStdoutStream.readLine();
            if (line.isNull()) {
                break;
            } else {
                tbStage1Output->append(line);
                refreshDirectory();
            }
        }
}
void MainWindow::updateStage1TextError() {
    QTextStream rsyncStdoutStream(process->readAllStandardError());
        while (true)
        {
            QString line = rsyncStdoutStream.readLine();
            if (line.isNull()) {
                break;
            } else {
                tbStage1Output->append(line);
                refreshDirectory();
            }
        }
}

void MainWindow::setStage1Elements(bool status) {
    sbRLGly->setEnabled(status);
    sbRLNonGly->setEnabled(status);
    leCutoff->setEnabled(status);
}

void MainWindow::executeStage1() {
    try {
        if (ACTIVE_DIR.isEmpty()) {
            throw std::runtime_error("REDCRAFT Path is empty. Please select a valid directory using File->Open Folder...");
        }
        QMessageBox::Button reply = QMessageBox::question(this, "Are you sure?", "Are you sure you want to execute stage 1?");
        if (reply==QMessageBox::Yes) {

            // First let's clear the output
            tbStage1Output->clear();

            // We must get the options from the boxes
            QStringList params;
                params << RDC_PREFIX
                          // It goes 1, 2, all
                       << (RAM_NONGLY < 3 ? QString::number(RAM_NONGLY) : "all")
                          // Bizarrely, this one goes 0, 1, all
                       << (RAM_GLY < 2 ? QString::number(RAM_GLY) : "all")
                       << (CUTOFF > -10000 ? QString::number(CUTOFF) : "");
            //if (RAMANGLES) {
            //    params << RDC_PREFIX
            //           << "--custom-ramangles";
            //} else {
            //    params << RDC_PREFIX
            //              // It goes 1, 2, all
            //           << (RAM_NONGLY < 3 ? QString::number(RAM_NONGLY) : "all")
                          // Bizarrely, this one goes 0, 1, all
            //           << (RAM_GLY < 2 ? QString::number(RAM_GLY) : "all")
            //           << (CUTOFF > -10000 ? QString::number(CUTOFF) : "");

            //}



            if (process) delete process;
            process = new QProcess(this);
            process->setWorkingDirectory(ACTIVE_DIR.path());
            // Create a signal to allow updating the textbox in real time
            connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::updateStage1Text);
            connect(process, &QProcess::readyReadStandardError, this, &MainWindow::updateStage1TextError);
            QString p = REDCRAFT_PATH + "/redcraft/bin/stage1";
            process->start(p, params);
            if (!process->waitForStarted(1500)) {
                throw std::runtime_error("Unable to start stage1. Is it properly installed?");
            }
            refreshDirectory();
        }

    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::on_btStopStage1_clicked()
{
    if (process->pid() > 0) {
#ifdef WIN32
        process->kill();
#else
        process->terminate();
#endif
        tbStage1Output->append("\nStage 1 process terminated.");
    }
}
