#include "mainwindow.h"

QString getFilters(QListWidget* lw) {
    QString ret = "\
constraints: \n\
  filter: \n";
    for (int i = 0; i < lw->count(); i++) {
        auto item = lw->item(i);
        ret += QString("    - %1\n").arg(item->text());
    }
    return ret;
}

void MainWindow::updateStage0Text() {
    QTextStream rsyncStdoutStream(process->readAllStandardOutput());
        while (true)
        {
            QString line = rsyncStdoutStream.readLine();
            if (line.isNull()) {
                break;
            } else {
                tbStage0Output->append(line);
            }
        }
}


void MainWindow::updateStage0TextError() {
    QTextStream rsyncStdoutStream(process->readAllStandardError());
        while (true)
        {
            QString line = rsyncStdoutStream.readLine();
            if (line.isNull()) {
                break;
            } else {
                tbStage0Output->append(line);
            }
        }
}

void MainWindow::on_btExecuteStage0_clicked()
{


    // Create YAML file for pdbmine
    QString YAML =
"server:\n\
  uri: \"%1\"\n\
  port: %2 \n\
  poll: 10 \n\n\
%3\
query:\n\
 residueChain: \"%4\" \n\
 codeLength: 1 \n\
 windowSize: %5 \n\n\
results:\n\
  directory: \"%6\" \n\n\
method:\n\
  k-means:\n\
   k: %7";
    QString formattedYAML = YAML.arg(
        leURI->text(),
        "443",
        getFilters(lwFilterProteins),
        leSequence->text(),
        leWindowSize->text(),
        ".",
        leClusterSize->text()
                );


    try {
        if (ACTIVE_DIR.isEmpty()) {
            throw std::runtime_error("REDCRAFT Path is empty. Please select a valid directory using File->Open Folder...");
        }
        QMessageBox::Button reply = QMessageBox::question(this, "Are you sure?", "Are you sure you want to execute stage 0?");
        if (reply==QMessageBox::Yes) {


            tbStage0Output->clear();


            // we determine whether to use pdbmine or pdbmine2
            // pdbmine2 only uses tarballs

            QStringList params;
            if (!cbLoadFromTarball->isChecked()) {
            // Ensure that all required fields are non-empty
                if (leURI->text() == "" || leSequence->text() == "" || leClusterSize->text() == "" || leWindowSize->text() == "") {
                    throw std::runtime_error("Please make sure to populate all fields (filtered proteins not necessary)");
                }
                // Write config file in active_dir
                QString yamlPath = ACTIVE_DIR.path() + "/stage0_config.yaml";
                qDebug() << yamlPath;
                QFile file(yamlPath);
                    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
                        return;

                    QTextStream out(&file);
                    out << formattedYAML;
                file.close();

                // We must get the options from the boxes
                params <<  REDCRAFT_PATH + "/redcraft/scripts/pdbmine.py" <<
                          "-y" << yamlPath <<
                          "-l" << "INFO";

            } else {
                if (leTarballPath->text() == "") {
                    throw std::runtime_error("Please make sure to include a path to the tarball.");
                }
                QString filter = "";
                for (int i = 0; i < lwFilterProteins->count(); i++) {
                    auto item = lwFilterProteins->item(i);
                    filter += item->text() + " ";
                }
                params <<  REDCRAFT_PATH + "/redcraft/scripts/pdbmine2.py" <<
                           "--resolution" << leResolution->text() <<
                           "--filter" << filter <<
                           leTarballPath->text() <<
                           ACTIVE_DIR.path();

                qDebug() << params;

            }

            tbStage0Output->append("Beginning stage 0:\n");


            if (process) delete process;
            process = new QProcess(this);
            process->setWorkingDirectory(ACTIVE_DIR.path());
            // Create a signal to allow updating the textbox in real time
            connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::updateStage0Text);
            connect(process, &QProcess::readyReadStandardError, this, &MainWindow::updateStage0TextError);
            connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                    [=](int exitCode, QProcess::ExitStatus exitStatus){ /* ... */
                try {

                    switch(exitCode) {
                        case 0:
                            cbRamAngles->setEnabled(true);
                            setStage1Elements(false);
                            RAMANGLES = true;
                            refreshDirectory();
                            break;
                        case 1:
                            throw std::runtime_error("Error generated within python script. Perhaps you are missing imports?");
                            break;
                        case 2:
                            throw std::runtime_error("Invalid parameters passed to stage 0");
                            break;
                        case 3:
                            throw std::runtime_error("Missing parameters passed to stage 0");
                            break;
                        case 4:
                            throw std::runtime_error("Bad YAML configuration file");
                            break;
                        case 6:
                            throw std::runtime_error("Missing K-means clustering parameter");
                            break;
                    }
                    } catch (const std::exception &e) {
                        QMessageBox::warning(this, "Error", e.what());
                    }
            }
            );
            QString p = PYTHON_PATH;
            process->start(p, params);
            if (!process->waitForStarted(1500)) {
                throw std::runtime_error("Unable to start stage 0. Is it properly installed?");
            }
            refreshDirectory();
        }

    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }

}

void MainWindow::on_btInferSequence_clicked()
{
    inferSequence();
}

void MainWindow::on_btRemoveFilter_clicked()
{
    for (auto item : lwFilterProteins->selectedItems()) {
        delete item;
    }
}

void MainWindow::on_btAddFilter_clicked()
{
    QString itemText = QInputDialog::getText(this, "Filter Protein", "Protein to exclude");
    lwFilterProteins->addItem(itemText);
}

void MainWindow::on_btStopStage0_clicked()
{
    if (process->pid() > 0) {
#ifdef WIN32
        process->kill();
#else
        process->terminate();
#endif
        tbStage0Output->append("\nStage 0 process terminated.");
    }
}
