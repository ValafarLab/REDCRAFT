#include "mainwindow.h"


void MainWindow::findRDC() {
    try {
        // Attempt to find the RDC files at the updated path
        if (ACTIVE_DIR.isEmpty() || !QDir(ACTIVE_DIR.path()).exists()) {
            throw std::runtime_error(INVALID_PATH.toStdString());
        }
        QDir *redcraftDir = new QDir(ACTIVE_DIR.path());
        redcraftDir->setFilter(QDir::Files);
        QFileInfoList list = redcraftDir->entryInfoList();
        // List each item, and attempt to find an RDC prefix
        QRegularExpression fileRe("^(.+)\\.\\d+$");
        for (int i = 0; i < list.size(); i++) {
            QFileInfo info = list.at(i);
            QString fileName = info.fileName();
            QRegularExpressionMatch match = fileRe.match(fileName);
            if (match.hasMatch()) {
                QString matched = match.captured(1);
                // We have an RDC prefix
                RDC_PREFIX = matched;
                leRDC->setText(RDC_PREFIX);
                break;
                }
        }
        delete redcraftDir;
        if (RDC_PREFIX.compare(DEFAULT_RDC_PREFIX)!=0) {
            tbInfo->append("\nInterpreted RDC Prefix: " + RDC_PREFIX);
        }
        analyzeRDC();
        refreshDirectory();


    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::inferSequence() {
    try {
        // Must have a valid RDC to infer sequence`
        if (RDC_PREFIX.compare(DEFAULT_RDC_PREFIX)!=0) {
                QString command = PYTHON_PATH;
                qDebug() << command;
                QStringList params;
                params << REDCRAFT_PATH + "/redcraft/scripts/getsequence.py";
                QProcess* inferProcess = new QProcess(this);
                inferProcess->setWorkingDirectory(ACTIVE_DIR.path());
                params << ACTIVE_DIR.path() + "/" + RDC_PREFIX + ".1";
                connect(inferProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                    [=](int exitCode, QProcess::ExitStatus exitStatus){ /* ... */
                    qDebug() << exitStatus;
                    if (exitStatus != QProcess::NormalExit) {
                        throw std::runtime_error("Unable to run inference script. Is REDCRAFT properly installed?");
                    } else {
                        QString sequence = inferProcess->readAll();
                        leSequence->setText(sequence);
                    }
                });
                inferProcess->start(command, params);
                if (!inferProcess->waitForStarted(1500)) {
                    throw std::runtime_error("Unable to run inference script. Is REDCRAFT properly installed?");
                }
        } else {
            throw std::runtime_error("Must open workspace with valid RDC file to infer sequence.");
        }
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::refreshDirectory() {
    // Attempt to refresh the directory listing
    try {
        if (ACTIVE_DIR.isEmpty()) {
            throw std::runtime_error(INVALID_PATH.toStdString());
        }
        QDir *redcraftDir = new QDir(ACTIVE_DIR.path());
        redcraftDir->setFilter(QDir::Files);

        // We will implement natural sorting with QCollator
        redcraftDir->setSorting(QDir::NoSort);
        auto list = redcraftDir->entryList();
        QCollator collator;
        // This way we sort via 1,2,...10 instead of 1,10,19...2
        collator.setNumericMode(true);
        std::sort(list.begin(), list.end(), collator);
        lwFileList->clear();
        for (int i = 0; i < list.size(); i++) {
            QFileInfo info = list.at(i);
            QString fileName = info.fileName();
            // depending on the filetype, we will use separate icons
            QListWidgetItem* item;
            if (fileName.endsWith(".pdb")) {
                item = new QListWidgetItem(QIcon(":icons/pdb.svg"), fileName);
            } else if (fileName.endsWith(".out")) {
                item = new QListWidgetItem(QIcon(":icons/out.svg"), fileName);
            } else if (fileName.startsWith(RDC_PREFIX)) {
                item = new QListWidgetItem(QIcon(":icons/rdc.svg"), fileName);
            } else if (fileName.endsWith(".angles")) {
                item = new QListWidgetItem(QIcon(":icons/angles.svg"), fileName);
            } else if (fileName.endsWith(".ramangles")) {
                item = new QListWidgetItem(QIcon(":icons/pdbmine.svg"), fileName);
            }
            else {
                item = new QListWidgetItem(fileName);
            }
            lwFileList->addItem(item);
        }
        delete redcraftDir;
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::calculateResidues() {
    try {
        // Counts the residues by counting the number of lines and dividing by 7
        QFile f(ACTIVE_DIR.path() + "/" + RDC_PREFIX + ".1");
        int total = 0;
        if (f.open(QIODevice::ReadOnly)) {
            QTextStream t(&f);
            while (!t.atEnd()) {
                t.readLine();
                total++;
            }
            total /= 7;
            // Update the stop residues box
            sbStopResidue->setValue(total);
        } else {
            throw std::runtime_error(("Unable to find RDC file at " + RDC_PREFIX + ".1").toStdString());

        }

    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

bool MainWindow::openFolderDialog() {
    QFileDialog *dialog = new QFileDialog(nullptr, "Open REDCRAFT Directory", ACTIVE_DIR.path());
    dialog->setFileMode(QFileDialog::DirectoryOnly);
    dialog->setOption(QFileDialog::ShowDirsOnly, false);
//    dialog->setOption(QFileDialog::ShowDirsOnly);
    int code = dialog->exec();
    if (code == QDialog::Accepted) {
        QString p = dialog->directory().path();
        return this->openFolder(p);
    }
    return false;
}

bool MainWindow::openFolder(const QString &path) {
        ACTIVE_DIR = path;
        if (!ACTIVE_DIR.isEmpty()) {
            statusBar->showMessage("Folder loaded: " + QFileInfo(ACTIVE_DIR.path()).canonicalFilePath());
            refreshDirectory();
            readConfig();
            findRDC();
            updateRecentsInMemory(path);
            return true;
        }
    return false;
}

void MainWindow::analyzeRDC() {
    // We must load from all alignment media
    // and store all values into lists
    QList<QMap<QString, QList<double>>> consolidated_residues_total;
    QList<QList<int>> residues_total;
    QList<QList<double>> media_total;
    tbRDCInfo->clear();

    for (int i = 1; i <= MEDIA_COUNT; i++) {
        QString path = ACTIVE_DIR.path() + "/" + RDC_PREFIX + "." + QString::number(i);
        // Get some analytics about the currently loaded RDC
        QMap<QString, QList<double>> consolidated_residues;
        QList<int> residues;
        QList<double> media;
        QFile inputFile(path);
        QRegularExpression residueRE("^([A-z]*)\\s+(-?\\d+\\.?\\d+)\\s+");
        QRegularExpression internalRE("^\\d+\\s+[A-Z]+\\s+[A-Z]+\\s+\\d+\\s+[A-Z]+\\s+[A-Z]+\\s+(-?\\d+\\.?\\d+)\\s+\\d+");

        int res_count = 0;
        int unknown_count = 0;
        int main_res = 0;
        int err_res = 0;

        // We will analyze the RDC file line by line, in clusters of 7, in order to count the number of unknown bond readings
        // Symbolized by 999
        if (inputFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&inputFile);
            while (!in.atEnd()) {
                QString line = in.readLine();
                // Here we are starting a new res
                if (main_res == 0) {
                    QRegularExpressionMatch match = residueRE.match(line);
                    if (match.hasMatch()) {
                        res_count++;
                        main_res = (main_res + 1) % 7;
                        QString residue = match.captured(1);
                        QString err = match.captured(2);
                        double errD = err.toDouble();
                        if (errD == 999) {
                            unknown_count++;
                        }
                        if (consolidated_residues.contains(residue)) {
                            QList<double> vals = consolidated_residues[residue];
                            vals.append(errD);
                            consolidated_residues[residue] = vals;
                        } else {
                            QList<double> vals;
                            vals.append(errD);
                            consolidated_residues[residue] = vals;
                        }
                    }
                }
                else if (main_res < 6) {
                    QRegularExpressionMatch match = internalRE.match(line);
                    if (match.hasMatch()) {
                        main_res = (main_res + 1) % 7;
                        QString err = match.captured(1);
                        double errD = err.toDouble();
                        if (errD == 999) {
                            err_res++;
                        }
                    }
                } else {
                    // Finally we must add the final value
                    QRegularExpressionMatch match = internalRE.match(line);
                    if (match.hasMatch()) {
                        main_res = (main_res + 1) % 7;
                        QString err = match.captured(1);
                        double errD = err.toDouble();
                        if (errD == 999) {
                            err_res++;
                        }
                        residues.append(res_count);
                        media.append(err_res);
                        err_res = 0;
                    }

                }
            }
        }
        QList<QString> keys = consolidated_residues.keys();
        QString longest_key = "NONE";
        int longest = 0;
        for (QString key : keys) {
            int l = consolidated_residues[key].length();
            if (l > longest){
                longest_key = key;
                longest = l;
            }
        }
        QString stats = "RDC Statistics for %1, media %2:\n\n"
                        "• Total residues: %3\n"
                        "• Unique amino acids: %4\n"
                        "• Non-valid residues: %5\n"
                        "• Non-valid percentage: %6%\n"
                        "• Most frequent amino acid: %7, which occurs %8 times\n";
        QString statsf = QString(stats).arg(
                        RDC_PREFIX,
                        QString::number(i),
                        QString::number(res_count),
                        QString::number(keys.length()),
                        QString::number(unknown_count),
                        QString::number((double(unknown_count)/res_count)*100),
                        longest_key,
                        QString::number(longest)
                    );
        tbRDCInfo->append(statsf);
        consolidated_residues_total.append(consolidated_residues);
        residues_total.append(residues);
        media_total.append(media);
    }

    bool consolidated = cbConsolidateRes->isChecked();
    histogram->loadResidues(consolidated, consolidated_residues_total, residues_total, media_total);
    dwRDCProfile->show();
}

void MainWindow::clearAngles() {
    try {
        if (ACTIVE_DIR.isEmpty()) {
            throw std::runtime_error(INVALID_PATH.toStdString());
        }
        QMessageBox::Button reply = QMessageBox::question(this, "Are you sure?", "Are you sure you want to delete all .angles files?");
        if (reply==QMessageBox::Yes) {
            QDir dir(ACTIVE_DIR.path());
            dir.setNameFilters(QStringList() << "*.angles");
            dir.setFilter(QDir::Files);
            foreach(QString dirFile, dir.entryList()) {
                dir.remove(dirFile);
            }
            refreshDirectory();
        }

    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::clearRamAngles() {
    try {
        if (ACTIVE_DIR.isEmpty()) {
            throw std::runtime_error(INVALID_PATH.toStdString());
        }
        QMessageBox::Button reply = QMessageBox::question(this, "Are you sure?", "Are you sure you want to delete all .angles files?");
        if (reply==QMessageBox::Yes) {
            QDir dir(ACTIVE_DIR.path());
            dir.setNameFilters(QStringList() << "*.ramangles");
            dir.setFilter(QDir::Files);
            foreach(QString dirFile, dir.entryList()) {
                dir.remove(dirFile);
            }
            refreshDirectory();
        }

    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }

}

void MainWindow::clearOutPDB() {
    try {
        if (ACTIVE_DIR.isEmpty()) {
            throw std::runtime_error(INVALID_PATH.toStdString());
        }
        QMessageBox::Button reply = QMessageBox::question(this, "Are you sure?", "Are you sure you want to delete all .out, .redcat.m* and .pdb files?");
        if (reply==QMessageBox::Yes) {
            QDir dir(ACTIVE_DIR.path());
            dir.setNameFilters(QStringList() << "*.out" << "*.redcat.m*" << "*.pdb");
            dir.setFilter(QDir::Files);
            foreach(QString dirFile, dir.entryList()) {
                dir.remove(dirFile);
            }
            refreshDirectory();
        }

    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::convertRDC() {
    try {
        if (ACTIVE_DIR.isEmpty()) {
            throw std::runtime_error(INVALID_PATH.toStdString());
        }
        QMessageBox::Button reply = QMessageBox::question(this, "Are you sure?", "Are you sure you want to convert your legacy RDC files to the new format?");
        if (reply==QMessageBox::Yes) {
            QString command = PYTHON_PATH;
            qDebug() << command;
            QStringList params;
            params << REDCRAFT_PATH + "/redcraft/scripts/convertoldrdc.py";
            QProcess* converterProcess = new QProcess(this);
            converterProcess->setWorkingDirectory(ACTIVE_DIR.path());
            params << ACTIVE_DIR.path() << RDC_PREFIX << (RDC_PREFIX + "_new");
            qDebug() << params;
            converterProcess->start(PYTHON_PATH, params);
            QString result = converterProcess->readAll();
            qDebug() << result;
            if (!converterProcess->waitForStarted(1500)) {
                throw std::runtime_error("Unable to run conversion script. Is REDCRAFT properly installed?");
            }
            refreshDirectory();
            if (result == "") {
                QMessageBox::information(this, "Success", "Successfully converted RDC. New file name: " + RDC_PREFIX + "_new.");
            } else {
                QMessageBox::warning(this, "Result", result);
            }
            this->leRDC->setText(RDC_PREFIX+"_new");
        }

    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::updateRecentsInMemory(const QString &recent) {
    QSettings settings;
    QStringList recentFilePaths =
            settings.value("recentFiles").toStringList();
    recentFilePaths.removeAll(recent);
    recentFilePaths.prepend(recent);
    while (recentFilePaths.size() > MAX_RECENTS)
        recentFilePaths.removeLast();
    settings.setValue("recentFiles", recentFilePaths);

    updateRecentsView();
}

void MainWindow::updateRecentsView() {
    QSettings settings;
    QStringList recentFilePaths =
            settings.value("recentFiles").toStringList();

    recentFolders->clear();

    for (auto i = 0; i < recentFilePaths.size(); i++) {
        QAction* action = new QAction();
        QString path = recentFilePaths.at(i);
        action->setText(path);
        connect(action, &QAction::triggered, [=, this]() {
            this->openFolder(path);
        });
        recentFolders->addAction(action);
    }
}


