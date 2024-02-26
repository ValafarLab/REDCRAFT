/**
  * MainWindow.cpp
  * Julian Rachele
  * This file is a part of REDCRAFT
  *
  */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QString path) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIntValidator* validator = new QIntValidator(this);
    QDoubleValidator *validator2=new QDoubleValidator(0,10000000,2,this);
    validator2->setNotation(QDoubleValidator::ScientificNotation);
    ui->StartlineEdit->setValidator(validator);
    ui->StoplineEdit->setValidator(validator);
    ui->sbClusterCount->setValidator(validator);
    ui->sbClusterSensitivity->setValidator(validator2);
    ui->sbLJThreshold->setValidator(validator2);
    ui->sbMax->setValidator(validator);


    statusBar = ui->statusBar;
    statusBar->showMessage("No folder selected.");


    // Initialize general elements
    lwFileList = this->findChild<QListWidget *>("lwFileList");

    // For the file list, we want to add a custom context menu
    // that will allow for certain properties



    tbInfo = this->findChild<QTextBrowser *>("tbInfo");
    tbInfo->setText(DEFAULT_MESSAGE);
    tabWidget = this->findChild<QTabWidget *>("tabWidget");
    tabWidget->setCurrentIndex(1);
    recentFolders = this->findChild<QMenu *>("menuOpen_Recent");

    dwRDCProfile = this->findChild<QDockWidget *>("dwRDCProfile");
    dwRDCProfile->hide();
    histogram = this->findChild<Histogram *>("histogram");
    tbRDCInfo = this->findChild<QTextBrowser *>("tbRDCInfo");
    cbConsolidateRes = this->findChild<QCheckBox *>("cbConsolidateRes");

    // Initialize stage 0 elements
    cbLoadFromTarball = this->findChild<QCheckBox *>("cbLoadFromTarball");
    btLoadFromTarball = this->findChild<QPushButton *>("btLoadFromTarball");
    leTarballPath = this->findChild<QLineEdit *>("leTarballPath");



    leURI = this->findChild<QLineEdit *>("leURI");
    leSequence = this->findChild<QLineEdit *>("leSequence");
    leWindowSize = this->findChild<QLineEdit *>("leWindowSize");
    leClusterSize = this->findChild<QLineEdit *>("leClusterSize");
    leResolution = this->findChild<QLineEdit *>("leResolution");

    lwFilterProteins = this->findChild<QListWidget *>("lwFilterProteins");
    tbStage0Output = this->findChild<QTextBrowser *>("tbStage0Output");

    btInferSequence = this->findChild<QPushButton *>("btInferSequence");
    btAddFilter = this->findChild<QPushButton *>("btAddFilter");
    btRemoveFilter = this->findChild<QPushButton *>("btRemoveFilter");


    // Initialize relevant stage 1 elements
    tbStage1Output = this->findChild<QTextBrowser *>("tbStage1Output");
    cbRamAngles = this->findChild<QCheckBox *>("cbRamAngles");
    cbRamAngles->setVisible(false);
    leRDC = this->findChild<QLineEdit *>("leRDC");
    leCutoff = this->findChild<QLineEdit *>("leCutoff");
    sbRLGly = this->findChild<QSpinBox *>("sbRLGly");
    sbRLNonGly = this->findChild<QSpinBox *>("sbRLNonGly");

    // Stage 2 basic elements
    tbStage2Output = this->findChild<QTextBrowser *>("tbStage2Output");
    leSearchDepth = this->findChild<QLineEdit *>("leSearchDepth");
    cbRunType = this->findChild<QCheckBox *>("cbRunType");

    // Stage 2 advanced elements
    btAddSearchDepth = this->findChild<QPushButton *>("btAddSearchDepth");
    twDepthSearch = this->findChild<QTableWidget *>("twDepthSearch");
    dsbScoreThreshold = this->findChild<QLineEdit *>("dsbScoreThreshold");
    sbClusterSensitivity = this->findChild<QLineEdit *>("sbClusterSensitivity");
    sbLJThreshold = this->findChild<QLineEdit *>("sbLJThreshold");
   /* sbDecimationStartResidue = this->findChild<QSpinBox *>("sbDecimationStartResidue");
    sbDecimationStopResidue = this->findChild<QSpinBox *>("sbDecimationStopResidue");*/
    leRefinementScript = this->findChild<QLineEdit *>("leRefinementScript");

    lbREDCATStatus = this->findChild<QLabel *>("lbREDCATStatus");
    btFindREDCAT = this->findChild<QPushButton *>("btFindREDCAT");
    twREDCAT = this->findChild<QTableWidget *>("twREDCAT");
    leREDCATOutput = this->findChild<QLineEdit *>("leREDCATOutput");
    leSamplingsNull = this->findChild<QLineEdit *>("leSamplingsNull");
    leSamplingsError = this->findChild<QLineEdit *>("leSamplingsError");
    leSearchRange = this->findChild<QLineEdit *>("leSearchRange");

    twREDCAT->setColumnHidden(0, true);



    openGLWidget = this->findChild<PDBViewer *>("openGLWidget");
    tabs = this->findChild<QTabWidget *>("twStage2Settings");
    tabs->setCurrentIndex(0);

    sbStartResidue = this->findChild<QSpinBox *>("sbStartResidue");
    sbStopResidue = this->findChild<QSpinBox *>("sbStopResidue");
    sbMedia = this->findChild<QSpinBox *>("sbMedia");

    cbEnableDecimation = this->findChild<QCheckBox *>("cbEnableDecimation");
    cbEnableRefinement = this->findChild<QCheckBox *>("cbEnableRefinement");

    process = new QProcess(this);

    if (!path.isEmpty()) {
        openFolder(path);
    }

    updateRecentsView();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete tbInfo;
    delete tbStage1Output;
    delete tbStage2Output;
    delete lwFileList;
    delete leSearchDepth;
    delete leRDC;
    delete leURI;
    delete cbRunType;
}



/*
 *  Below are all slot commands, or commands triggered by
 *  interacting with particular UI elements
 */


void MainWindow::on_actionOpen_Folder_triggered()
{
    openFolderDialog();
}
void MainWindow::on_actionOpen_RMSD_triggered()
{
    openRMSD();
}

void MainWindow::on_btClearAngleFiles_clicked()
{
    clearAngles();
}

void MainWindow::on_btClearOutPDB_clicked()
{
    clearOutPDB();
}

void MainWindow::on_btRunStage2_clicked()
{
    tabs->setCurrentIndex(0);
    executeStage2();
}

void MainWindow::on_btRefreshFiles_clicked()
{
    refreshDirectory();
}

void MainWindow::on_btUseAllResidues_clicked()
{
    calculateResidues();
}

void MainWindow::on_actionGenerate_PDB_triggered()
{
    executeStage2();
}

void MainWindow::on_btRunStage1_clicked()
{
    executeStage1();
}

void MainWindow::on_actionProcess_RDC_triggered()
{
    findRDC();
}

void MainWindow::on_btFindRDC_clicked() {
    findRDC();
}

void MainWindow::on_actionConvert_old_RDC_to_new_triggered()
{
    convertRDC();
}

void MainWindow::on_leCutoff_textChanged(const QString &arg1)
{
    if (arg1.toLower() == "none") {
        CUTOFF = -10000;
    }
    try {
        CUTOFF = arg1.toDouble();
    } catch (const std::exception &) {
        CUTOFF = -10000;
    }
}

void MainWindow::on_leSearchDepth_textChanged(const QString &arg1)
{
    try {
        SEARCH_DEPTH = arg1.toInt();
    } catch (const std::exception &) {
        // if a parsing error occurs, reset to default
        SEARCH_DEPTH = 100;
        leSearchDepth->setText("100");
    }
}



void MainWindow::on_lwFileList_itemDoubleClicked(QListWidgetItem *item)
{
    try {
    if (item->text().endsWith(".pdb")) {
        QString program = "pymol";
        QStringList arguments;
        arguments << (QFileInfo(ACTIVE_DIR.path()).canonicalFilePath() + "/" + item->text());
        QProcess *p = new QProcess(this);
        p->start(program, arguments);
        if (!p->waitForStarted(1500)) {
            throw std::runtime_error("Unable to open .pdb files. Please install pymol and make sure it is in your PATH.");
        }
    } else if (item->text().endsWith(".out")) {
        analysis = new AnalysisSuite(this);
        analysis->openForAnalysis(QFileInfo(ACTIVE_DIR.path()).canonicalFilePath(), item->text());
        analysis->show();

    } else if (item->text().endsWith(".redcat")) {
        parseREDCATFile(QFileInfo(ACTIVE_DIR.path()).canonicalFilePath() + "/" + item->text());
        tabWidget->setCurrentIndex(3);
    } else {
        // Open the file the default text editor
        if (!QDesktopServices::openUrl(QFileInfo(ACTIVE_DIR.path()).canonicalFilePath() + "/" + item->text())){
            throw std::runtime_error(QString("Unable to open " + item->text() + ". Please be sure your operating system has a default program for dealing with this file type.").toStdString());
        }
    }
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::on_twDepthSearch_cellDoubleClicked(int row, int column)
{
    QMessageBox::Button reply = QMessageBox::question(this, "Are you sure?", "Are you sure you want to remove this entry?");
    if (reply==QMessageBox::Yes) {
        twDepthSearch->removeRow(row);
    }
}

void MainWindow::on_cbEnableDecimation_stateChanged(int arg1)
{
    dsbScoreThreshold->setEnabled(arg1);
    if(ui->sbClusterCount->text()==""){
    sbClusterSensitivity->setEnabled(arg1);
    }
    ui->DecimationRangelineEdit->setEnabled(arg1);
    ui->sbMax->setEnabled(arg1);
    if(ui->sbClusterSensitivity->text()==""){
    ui->sbClusterCount->setEnabled(arg1);
    }
  /*  sbDecimationStartResidue->setEnabled(arg1);
    sbDecimationStopResidue->setEnabled(arg1);*/
    ENABLE_DECIMATION = arg1;
}

void MainWindow::on_btAddSearchDepth_clicked()
{
    dialogSearchDepth = new DialogSearchDepth(twDepthSearch, nullptr);
    dialogSearchDepth->show();
}

QString MainWindow::getDSSasString()
{
    QString s;
    for (int i = 0; i < twDepthSearch->rowCount(); i++) {
        s += QString("%1Residue_%2=%3\n").arg(
                    (twDepthSearch->item(i,0)->checkState() == Qt::CheckState::Checked) ? "" : "#",
                    twDepthSearch->item(i,1)->text(),
                    twDepthSearch->item(i,2)->text());
    }
    return s;
}


void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_cbEnableRefinement_stateChanged(int arg1)
{
    this->leRefinementScript->setEnabled(arg1);
    ENABLE_REFINEMENT = arg1;
}

void MainWindow::on_actionAbout_REDCRAFT_triggered()
{
    QMessageBox::about(this, "About REDCRAFT", "REDCRAFT is an open-source software tool for determining a protein’s structure using residual dipolar couplings (RDCs). It allows simultaneous determination of a protein’s structure and dynamics. Its effectiveness has been demonstrated on both synthetic and experimental data. REDCRAFT contains stages that allow the incorporation of user-specified dihedral angle constraints, such as those produced by TALOS or a restriction to specific regions of Ramachandran space. It is robust with respect to noise and missing data. The program is highly efficient and can produce a structure for an 80-residue protein within two hours.\n\nProject directed by Dr. Homayoun Valafar.\nGUI designed in Qt by Julian Rachele");
}

void MainWindow::on_actionREDCRAFT_Documentation_triggered()
{
    QDesktopServices::openUrl(QUrl("https://redcraft.readthedocs.io/"));
}

void MainWindow::on_leRDC_textChanged(const QString &arg1)
{
    // When the RDC value here is changed, *RDC_PREFIX must follow
    RDC_PREFIX = arg1;
}

void MainWindow::on_sbRLNonGly_valueChanged(int arg1)
{
    RAM_NONGLY = arg1;
}

void MainWindow::on_sbRLGly_valueChanged(int arg1)
{
    RAM_GLY = arg1;
}

void MainWindow::on_cbRunType_stateChanged(int arg1)
{
    RUN_TYPE = arg1;
}

void MainWindow::on_sbStartResidue_valueChanged(int arg1)
{
    START_RESIDUES = arg1;
}

void MainWindow::on_sbStopResidue_valueChanged(int arg1)
{
    STOP_RESIDUES = arg1;
}

void MainWindow::on_sbMedia_valueChanged(int arg1)
{
    MEDIA_COUNT = arg1;
}

/*void MainWindow::on_sbClusterSensitivity_valueChanged(int arg1)
{
    DECIMATION_CLUSTER_SENSITIVITY = arg1;
}*/

/*void MainWindow::on_dsbScoreThreshold_valueChanged(double arg1)
{
    DECIMATION_THRESHOLD = arg1;
}*/

/*void MainWindow::on_sbDecimationStartResidue_valueChanged(int arg1)
{
    DECIMATION_START_RESIDUE = arg1;
}

void MainWindow::on_sbDecimationStopResidue_valueChanged(int arg1)
{
    DECIMATION_STOP_RESIDUE = arg1;
}*/

void MainWindow::on_leRefinementScript_textChanged(const QString &arg1)
{
    REFINEMENT_SCRIPT = arg1;
}

void MainWindow::on_pbOpenDirectory_clicked()
{
    QUrl url = QUrl::fromLocalFile(QFileInfo(ACTIVE_DIR.path()).canonicalFilePath());
    QDesktopServices::openUrl(url);
}
void MainWindow::openRMSD(){
    try {
        if (ACTIVE_DIR.isEmpty()) {
            throw std::runtime_error("REDCRAFT Path is empty. Please select a valid directory using File->Open Folder...");
        }

            process2 = new QProcess(this);
            process2->setWorkingDirectory(ACTIVE_DIR.path()); 
            QString p = REDCRAFT_PATH + "/redcraft/bin/rmsd";
            process2->start(p, QStringList());
            if (!process2->waitForStarted(1500)) {
                throw std::runtime_error("Unable to start RMSD GUI. Is it properly installed?");
            }
           // refreshDirectory();
        }

     catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}
void MainWindow::on_PDBFinderButton_clicked(){
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("pdb(*.pdb)");
    QString file= dialog.getOpenFileName(0,"select PDB file",QString(),"PDB (*.pdb)");
    ui->PDBFilelineEdit->setText(file);
    ui->PDBFilelineEdit->displayText();
}
void MainWindow::updateRMSDText() {
    QTextStream rsyncStdoutStream(process3->readAllStandardOutput());
        while (true)
        {
            QString line = rsyncStdoutStream.readLine();
            if (line.isNull()) {
                break;
            } else {
                ui->RMSDtextBrowser->append(line);
                refreshDirectory();
            }
        }
}

void MainWindow::on_runRMSD_clicked()
{
    try{
        if (ACTIVE_DIR.isEmpty()) {
            throw std::runtime_error("REDCRAFT Path is empty. Please select a valid directory using File->Open Folder...");
        }
        if(ui->StartlineEdit->text().toInt()<=0||ui->StoplineEdit->text().toInt()<=0){
            throw std::runtime_error("Starting or Stopping out file is invalid");
        }
        if(ui->StartlineEdit->text().toInt()>ui->StoplineEdit->text().toInt()){
             throw std::runtime_error("Please enter vaild starting and stopping residue");
        }
        if(ui->PDBFilelineEdit->text()==""){
             throw std::runtime_error("Please type a PDB file or search for one using the 'Find PDB' button");
        }
        ui->RMSDtextBrowser->clear();
        QString start=ui->StartlineEdit->text();
        QString stop=ui->StoplineEdit->text();
        QString file=ui->PDBFilelineEdit->text();
        QStringList params;
        params << start << stop << file;
        QString p = REDCRAFT_PATH + "/redcraft/bin/rmsd";
        process3 = new QProcess(this);
        process3->setWorkingDirectory(ACTIVE_DIR.path());
        connect(process3, &QProcess::readyReadStandardOutput, this, &MainWindow::updateRMSDText);
        connect(process3, &QProcess::readyReadStandardError, this, &MainWindow::updateRMSDText);
        process3->start(p, params);
        if (!process3->waitForStarted(1500)) {
            throw std::runtime_error("Unable to start RMSD binary. Is it properly installed?");
        }

    }
    catch (const std::exception &e) {
       QMessageBox::warning(this, "Error", e.what());
   }

}

void MainWindow::on_sbClusterSensitivity_textChanged(const QString &arg1)
{

    if(arg1!=""){
        ui->sbClusterCount->setEnabled(false);
    }
    else{
        ui->sbClusterCount->setEnabled(true);
    }
}

void MainWindow::on_sbClusterCount_textChanged(const QString &arg1)
{
    if(arg1!=""){
        ui->sbClusterSensitivity->setEnabled(false);
    }
    else{
        ui->sbClusterSensitivity->setEnabled(true);
    }
}

void MainWindow::on_cbConsolidateRes_stateChanged(int arg1)
{
    analyzeRDC();
}

void MainWindow::on_sbLJThreshold_textChanged(const QString &arg1)
{

    try {
        LJ_THRESHOLD = arg1.toDouble();
    } catch (const std::exception &) {
        // if a parsing error occurs, reset to default
        LJ_THRESHOLD = 50.0;
        sbLJThreshold->setText("50.0");
    }

}



void MainWindow::on_cbRamAngles_stateChanged(int arg1)
{
    setStage1Elements(!arg1);
    RAMANGLES = arg1;
}

void MainWindow::on_cbLoadFromTarball_stateChanged(int arg1)
{
    // Enable options for loading from tarball, and set the default stage 0 operation to use pdbmine2.py
    // Otherwise, disable options

    btLoadFromTarball->setEnabled(arg1);
    leTarballPath->setEnabled(arg1);
    leResolution->setEnabled(arg1);


    // Reverse for other options
    leURI->setEnabled(!arg1);
    leSequence->setEnabled(!arg1);
    leWindowSize->setEnabled(!arg1);
    leClusterSize->setEnabled(!arg1);
    btInferSequence->setEnabled(!arg1);

}

void MainWindow::on_btLoadFromTarball_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Tarball (*.tar)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString filePath;
    QStringList files;
    if (dialog.exec()) {
        files = dialog.selectedFiles();
        if (!files.empty()) {
            filePath = files[0];
            qDebug() << filePath;
            leTarballPath->setText(filePath);
            TARBALL_PATH = filePath;
        }
    }
}


void MainWindow::on_btClearRamAngles_clicked()
{
    clearRamAngles();
}
