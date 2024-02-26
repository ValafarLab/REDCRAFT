#include "mainwindow.h"

void MainWindow::updateStage2Text() {
    QTextStream rsyncStdoutStream(process->readAllStandardOutput());
        while (true)
        {
            QString line = rsyncStdoutStream.readLine();
            if (line.isNull()) {
                break;
            } else {
                tbStage2Output->append(line);
                refreshDirectory();
            }
        }
}
void MainWindow::updateStage2TextError() {
    QTextStream rsyncStdoutStream(process->readAllStandardError());
        while (true)
        {
            QString line = rsyncStdoutStream.readLine();
            if (line.isNull()) {
                break;
            } else {
                tbStage2Output->append(line);
                refreshDirectory();
            }
        }
}

void MainWindow::executeStage2() {
    try {
        if (ACTIVE_DIR.isEmpty()) {
            throw std::runtime_error("REDCRAFT Path is empty. Please select a valid directory using File->Open Folder...");
        }
        QMessageBox::Button reply = QMessageBox::question(this, "Are you sure?", "Are you sure you want to execute stage 2 and overwrite the previous config file?");
        if (reply==QMessageBox::Yes) {

            // First let's clear the output
            tbStage2Output->clear();

            if (process) delete process;
            process = new QProcess(this);
            process->setWorkingDirectory(ACTIVE_DIR.path());
            writeConfig();
            //createNewConfig();
           formatConfig();
            // Create a signal to allow updating the textbox in real time
            connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::updateStage2Text);
            connect(process, &QProcess::readyReadStandardError, this, &MainWindow::updateStage2TextError);
            QString p = REDCRAFT_PATH + "/redcraft/bin/stage2";
            process->start(p, QStringList());
            if (!process->waitForStarted(1500)) {
                throw std::runtime_error("Unable to start stage2. Is it properly installed?");
            }
            refreshDirectory();
        }

    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::on_btStopStage2_clicked()
{ 
    tabs->setCurrentIndex(0);
    if (process->pid() > 0) {
#ifdef WIN32
        process->kill();
#else
        process->terminate();
#endif
        tbStage2Output->append("\nStage 2 process terminated.");
    }
}
