#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QTextStream>
using namespace std;

void MainWindow::readConfig() {
    // TODO: Don't hardcode the config path
    QString configPath = ACTIVE_DIR.path() + "/redcraft.conf";

    tbInfo->append("\nSuccessfully loaded config file at: " + configPath);
    QSettings settings(configPath, QSettings::IniFormat);

    // Begin taking apart the settings and setting them

    // Run settings
    settings.beginGroup("Run_Settings");
    if (settings.value("Run_Type") != QVariant::Invalid) {
        (settings.value("Run_Type").toString() == "continue") ? cbRunType->setChecked(true) : cbRunType->setChecked(false);
    }
    if (settings.value("RDC_File_Prefix") != QVariant::Invalid) {
        leRDC->setText(settings.value("RDC_File_Prefix").toString());
    }
    if (settings.value("Start_Residue") != QVariant::Invalid) {
        sbStartResidue->setValue(settings.value("Start_Residue").toInt());
    }
    if (settings.value("Stop_Residue") != QVariant::Invalid) {
        sbStopResidue->setValue(settings.value("Stop_Residue").toInt());
    }
    if (settings.value("Media_Count") != QVariant::Invalid) {
        sbMedia->setValue(settings.value("Media_Count").toInt());
    }
    if (settings.value("Default_Search_Depth") != QVariant::Invalid) {
        leSearchDepth->setText(settings.value("Default_Search_Depth").toString());
    }
    if (settings.value("LJ_Threshold") != QVariant::Invalid) {
        sbLJThreshold->setText(settings.value("LJ_Threshold").toString());
    }
    settings.endGroup();

    // Depth search settings
    settings.beginGroup("Depth_Search_Settings");
    QStringList keys = settings.childKeys();
    if (keys.size() > 0) {
        cbEnableDecimation->setChecked(true);
    }
    QRegularExpression keyRe("Residue_(\\d+)");
    for (auto &key : settings.childKeys()) {
        auto match = keyRe.match(key);
        if (match.hasMatch()) {
            // Find place to put new content
            auto res = match.captured(1);
            auto searchDepth = settings.value(key);
            if (searchDepth == QVariant::Invalid) {
                break;
            }
            for (int i = 0; i < twDepthSearch->rowCount(); i++) {
                if (twDepthSearch->item(i,1)->text() == res) {
                    // We have found the same item here
                    twDepthSearch->item(i,2)->setText(searchDepth.toString());
                    return;
                }
            }
            // Here it must be a new one
            QTableWidgetItem *enabled = new QTableWidgetItem("Enabled");
            enabled->setCheckState((key[0] != '#') ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
            QTableWidgetItem *residue = new QTableWidgetItem(res);
            QTableWidgetItem *depth = new QTableWidgetItem(searchDepth.toString());
            int row = twDepthSearch->rowCount();
            twDepthSearch->insertRow(row);
            int col = twDepthSearch->columnCount();
            if (col == 0) {
                twDepthSearch->insertColumn(0);
                twDepthSearch->insertColumn(1);
                twDepthSearch->insertColumn(2);
            }
            twDepthSearch->setItem(row, 0, enabled);
            twDepthSearch->setItem(row, 1, residue);
            twDepthSearch->setItem(row, 2, depth);
        }
    }

    settings.endGroup();


    // Decimation settings
    settings.beginGroup("Decimation_Settings");
    if (settings.value("Cluster_Sensitivity") != QVariant::Invalid) {
        // Enable decimation
        sbClusterSensitivity->setText(settings.value("Cluster_Sensitivity").toString());
        sbClusterSensitivity->displayText();
    }
    if (settings.value("Score_Threshold") != QVariant::Invalid) {
        dsbScoreThreshold->setText(settings.value("Score_Threshold").toString());
    }
    if (settings.value("Decimation_Ranges") != QVariant::Invalid) {
        //cout<<"Here"<<endl;
        cbEnableDecimation->setChecked(true);
        QVariant value=settings.value("Decimation_Ranges");
        QString strValue;
        if (value.type() == QVariant::StringList) {
          strValue = value.toStringList().join(",");
        } else {
          strValue = value.toString();
        }
        ui->DecimationRangelineEdit->setText(strValue);
        //qDebug()<<settings.value("Score_threshold").toString();
        ui->DecimationRangelineEdit->displayText();
       /* QRegularExpression re("(\\d+)-(\\d+)");
        QRegularExpressionMatch match = re.match(settings.value("Decimation_Ranges").toString());
        if (match.hasMatch()) {
            sbDecimationStartResidue->setValue(match.captured(1).toInt());
            sbDecimationStopResidue->setValue(match.captured(2).toInt());
        }*/
    }
    if (settings.value("Cluster_Count") != QVariant::Invalid) {
        ui->sbClusterCount->setText(settings.value("Cluster_Count").toString());
        ui->sbClusterCount->displayText();
    }
    if (settings.value("Maximum_Number_of_Additional_Structures") != QVariant::Invalid) {
       ui->sbMax->setText(settings.value("Maximum_Number_of_Additional_Structures").toString());
    }

    settings.endGroup();

    // Refinement selection
    settings.beginGroup("Refinement");
    if (settings.value("script1") != QVariant::Invalid) {
        cbEnableRefinement->setChecked(true);
        leRefinementScript->setText(settings.value("script1").toString());
    }
    settings.endGroup();
}

void MainWindow::writeConfig() {
    QString configPath = ACTIVE_DIR.path() + "/redcraft.conf";
    QFile c(configPath);
    c.remove();

    QSettings settings(configPath, QSettings::IniFormat);
    // Begin taking apart the settings and setting them

    // Run settings
    settings.beginGroup("Run_Settings");
    settings.setValue("Run_Type", RUN_TYPE ? "continue" : "new");
    settings.setValue("RDC_File_Prefix", RDC_PREFIX);
    settings.setValue("Start_Residue", START_RESIDUES);
    settings.setValue("Stop_Residue", STOP_RESIDUES);
    settings.setValue("Media_Count", MEDIA_COUNT);
    settings.setValue("Default_Search_Depth", SEARCH_DEPTH);
    settings.setValue("LJ_Threshold", LJ_THRESHOLD);
    QString dataPath = QString(".");
    settings.setValue("Data_Path", dataPath);
    settings.endGroup();

    // Depth search settings
    settings.beginGroup("Depth_Search_Settings");
    for (int row = 0; row < twDepthSearch->rowCount(); row++) {
        QString header = ((twDepthSearch->item(row,0)->checkState() == Qt::CheckState::Checked) ? QString("Residue_") : QString("#Residue_"))
                        + twDepthSearch->item(row,1)->text();
        settings.setValue(header, twDepthSearch->item(row,2)->text());
    }
    settings.endGroup();

    // Decimation settings
    settings.beginGroup("Decimation_Settings");
    if(ui->sbClusterSensitivity->text()==""||(ui->sbClusterSensitivity->isEnabled()==false&&ENABLE_DECIMATION==true)){
        settings.setValue("#Cluster_Sensitivity","");
    }
    else{
    QString enable_C = (ENABLE_DECIMATION ? "Cluster_Sensitivity" : "#Cluster_Sensitivity");
    settings.setValue(enable_C, ui->sbClusterSensitivity->text());
    }
    QString enable_S = (ENABLE_DECIMATION ? "Score_Threshold" : "#Score_Threshold");
    settings.setValue(enable_S, ui->dsbScoreThreshold->text());
   // QString range = QString("%1-%2").arg(QString::number(DECIMATION_START_RESIDUE), QString::number(DECIMATION_STOP_RESIDUE));
    QString range=ui->DecimationRangelineEdit->text();
    QString enable_D = (ENABLE_DECIMATION ? "Decimation_Ranges" : "#Decimation_Ranges");
    settings.setValue(enable_D, range);
    if(ui->sbClusterCount->text()==""||(ui->sbClusterCount->isEnabled()==false&&ENABLE_DECIMATION==true)){
        settings.setValue("#Cluster_Count","");
    }
    else{
    QString enable_cluster=(ENABLE_DECIMATION ? "Cluster_Count" : "#Cluster_Count");
    settings.setValue(enable_cluster,ui->sbClusterCount->text());
    }
    if(ui->sbMax->text()!=""){
    QString enable_Max=(ENABLE_DECIMATION ? "Maximum_Number_of_Additional_Structures" : "#Maximum_Number_of_Additional_Structures");
    settings.setValue(enable_Max,ui->sbMax->text());
    }
    else{
    settings.setValue("#Maximum_Number_of_Additional_Structures","");
    }
    settings.endGroup();

    // Refinement selection
    settings.beginGroup("Refinement");
    QString s = (ENABLE_REFINEMENT ? "script1" : "#script1");
    settings.setValue(s, REFINEMENT_SCRIPT);
    settings.endGroup();

    settings.disconnect();
    tbInfo->append("\nSuccessfully wrote config file at: " + configPath);
}
void MainWindow::createNewConfig(){
    try {
        if (ACTIVE_DIR.isEmpty()) {
            throw std::runtime_error("REDCRAFT Path is empty. Please select a valid directory using File->Open Workspace...");
        }

            process4 = new QProcess(this);
            process4->setWorkingDirectory(ACTIVE_DIR.path());
            QString p = REDCRAFT_PATH + "/redcraft/bin/stage2";
            process4->start(p + " --create-new", QStringList());
            if (!process4->waitForStarted(1500)) {
                throw std::runtime_error("Unable to start REDCRAFT binary. Is it properly installed?");
            }
           // refreshDirectory();
        }

     catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}
void MainWindow::formatConfig() {
    QString configPath = ACTIVE_DIR.path() + "/redcraft.conf";

    // Replace %23 with #
    QFile config(configPath);
    QString conf;
    if (config.open(QIODevice::ReadOnly)) {
        QTextStream stream(&config);
        conf = stream.readAll().replace("%23", "#");
    }
    config.remove();
    if (config.open(QIODevice::WriteOnly)) {
        QTextStream stream(&config);
        stream
               << "# REDCRAFT Auto-generated config file\n"
               << "# You are free to modify this file, but out of line comments will be overwritten.\n"
               << conf
               << "\n[OTEstimation]\n#Order Tensor estimation is currently not functioning.";
    }

}
