#include "mainwindow.h"


void MainWindow::on_btGenerateREDCAT_clicked()
{
    openPDB();
}

bool MainWindow::openPDB() {
    QString redcat_file = QFileDialog::getOpenFileName(this, tr("Open PDB File"), "", tr("Protein Data Bank files (*.pdb*);;All Files (*)"));
    if (redcat_file.isEmpty()) {
        return false;
    }
    return parsePDBFile(redcat_file);
}

bool MainWindow::parsePDBFile(QString pdb_file) {
    // Execute python script and autogenerate redcat file
    QProcess* process = new QProcess(this);
    process->setWorkingDirectory(ACTIVE_DIR.path());
    QString redcat_file = pdb_file + ".autogen.redcat";
    QStringList  params;
    params <<  REDCRAFT_PATH + "/redcat/scripts/createRedcatFile.py"
           << "--rdc" << pdb_file << pdb_file << redcat_file;
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus){ /* ... */
        try {

            if (exitCode == 0) {
                parseREDCATFile(redcat_file);
            } else {
                throw std::runtime_error(process->readAllStandardOutput() + process->readAllStandardError());
            }
            } catch (const std::exception &e) {
                QMessageBox::warning(this, "Error", e.what());
            }
    }
    );
    QString p = PYTHON_PATH;
    process->start(p, params);
    qDebug() << params;
    if (!process->waitForStarted(1500)) {
        throw std::runtime_error("Unable to start stage 0. Is it properly installed?");
    }
    refreshDirectory();
}

bool MainWindow::parseREDCATFile(QString redcat_file) {

    QFile file(redcat_file);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return false;
    }
    lbREDCATStatus->setText("Loaded: " + redcat_file);
    twREDCAT->setEnabled(true);
    twREDCAT->setRowCount(0);

    QTextStream in(&file);
    while(!in.atEnd()) {
        // String operations to read REDCAT file data
        QString line = in.readLine();
        QStringList l = line.split("/*");
        QString data = l[0].trimmed();
        QString comment = l[1].split("*")[0].trimmed();
        QStringList values = data.split(" ");

        // Now we create a table entry
        QTableWidgetItem *enabled = new QTableWidgetItem();
        enabled->setCheckState(Qt::CheckState::Unchecked);
        QTableWidgetItem *x1 = new QTableWidgetItem(values[0]);
        QTableWidgetItem *y1 = new QTableWidgetItem(values[1]);
        QTableWidgetItem *z1 = new QTableWidgetItem(values[2]);
        QTableWidgetItem *x2 = new QTableWidgetItem(values[3]);
        QTableWidgetItem *y2 = new QTableWidgetItem(values[4]);
        QTableWidgetItem *z2 = new QTableWidgetItem(values[5]);
        QTableWidgetItem *dipol = new QTableWidgetItem(values[6]);
        QTableWidgetItem *error = new QTableWidgetItem(values[7]);
        QTableWidgetItem *comments = new QTableWidgetItem(comment);
        int row = twREDCAT->rowCount();
        twREDCAT->insertRow(row);
        twREDCAT->setItem(row, 0, enabled);
        twREDCAT->setItem(row, 1, x1);
        twREDCAT->setItem(row, 2, y1);
        twREDCAT->setItem(row, 3, z1);
        twREDCAT->setItem(row, 4, x2);
        twREDCAT->setItem(row, 5, y2);
        twREDCAT->setItem(row, 6, z2);
        twREDCAT->setItem(row, 7, dipol);
        twREDCAT->setItem(row, 8, error);
        twREDCAT->setItem(row, 9, comments);


    }
    file.close();
    return true;
}
