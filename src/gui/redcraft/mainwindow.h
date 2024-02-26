#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCheckBox>
#include <QCollator>
#include <QDebug>
#include <QDesktopServices>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QProcess>
#include <QRegularExpression>
#include <QSettings>
#include <QSpinBox>
#include <QStatusBar>
#include <QString>
#include <QTabWidget>
#include <QTextBrowser>
#include <QTextStream>
#include <QTabWidget>

#include <algorithm>

#include "pdbviewer.h"
#include "analysissuite.h"
#include "dialogsearchdepth.h"
#include "histogram.h"

static QString DEFAULT_MESSAGE = "Welcome to REDCRAFT!\n\n"
                                "To begin, navigate to File->Open Workspace... and select a directory.\n\n"
                                "The directory should contain a number of files with the format <RDC header>.1, <RDC header>.2, etc.";
static QString DEFAULT_RDC_PREFIX = "RDC";
static QString DEFAULT_CONFIG = "[Run_Settings]\nRun_Type=%1\nStart_Residue=%2\nStop_Residue=%3\nMedia_Count=%4"
                                "\nData_Path=\".\"\nRDC_File_Prefix=%5\nDefault_Search_Depth=%6"
                                "\nLJ_Threshold=50.0\n[/Run_Settings]\n\n"
                                // Below is simply boilerplate, not implemented yet
                                "[Depth_Search_Settings]\n%7[/Depth_Search_Settings]\n\n"
                                "[Decimation_Settings]\n%8Cluster_Sensitivity=%9\n%8Score_Threshold=%10\n"
                                "%8Decimation_Ranges=%11-%12\n[/Decimation_Settings]\n\n[OTEstimation]\n"
                                "# syntax for OrderTensorEstimation is S?=Sxx Syy Sxy Sxz Syz\n#S1=Sxx Syy 0 0 0\n"
                                "#S2=Sxx2 Syy2 Sxy2 Sxz2 Syz2\n#Tolerance=1.0\n#Weight=1.0\n#Estimation_Range=5-25,40,42\n"
                                "#Dmax\n[/OTEstimation]\n\n[Refinement]\n%13script1=%14\n[/Refinement]";

static QString INVALID_PATH = "REDCRAFT Path is invalid. Please select a valid directory using File->Open Workspace...";

#ifdef WIN32
static QString REDCRAFT_PATH = "C:\\Program Files (x86)\\Redcraft\\";
#else
static QString REDCRAFT_PATH = "/usr/local/";
#endif

static int MAX_RECENTS = 5;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, QString path = "");
    ~MainWindow();
    QUrl ACTIVE_DIR;


private slots:
    void updateStage0Text();
    void updateStage0TextError();
    void updateStage1Text();
    void updateStage1TextError();
    void updateStage2Text();
    void updateStage2TextError();
    void updateRMSDText();
    void on_actionExit_triggered();
    void on_actionOpen_Folder_triggered();

    void on_actionOpen_RMSD_triggered();

    void on_actionProcess_RDC_triggered();

    void on_leRDC_textChanged(const QString &arg1);

    void on_btFindRDC_clicked();

    void on_btClearAngleFiles_clicked();

    void on_actionGenerate_PDB_triggered();

    void on_btRunStage1_clicked();

    void on_sbRLNonGly_valueChanged(int arg1);

    void on_sbRLGly_valueChanged(int arg1);

    void on_leCutoff_textChanged(const QString &arg1);

    void on_actionREDCRAFT_Documentation_triggered();

    void on_sbStartResidue_valueChanged(int arg1);

    void on_sbStopResidue_valueChanged(int arg1);

    void on_sbMedia_valueChanged(int arg1);

    void on_leSearchDepth_textChanged(const QString &arg1);

    void on_btClearOutPDB_clicked();

    void on_btRunStage2_clicked();

    void on_cbRunType_stateChanged(int arg1);

    void on_lwFileList_itemDoubleClicked(QListWidgetItem *item);

    void on_btRefreshFiles_clicked();

    void on_actionConvert_old_RDC_to_new_triggered();

    void on_actionAbout_REDCRAFT_triggered();

    void on_btUseAllResidues_clicked();

    void on_btStopStage1_clicked();

    void on_btStopStage2_clicked();

    void on_cbEnableDecimation_stateChanged(int arg1);

   // void on_sbClusterSensitivity_valueChanged(int arg1);

   // void on_dsbScoreThreshold_valueChanged(double arg1);

   /* void on_sbDecimationStartResidue_valueChanged(int arg1);

    void on_sbDecimationStopResidue_valueChanged(int arg1);*/

    void on_cbEnableRefinement_stateChanged(int arg1);

    void on_leRefinementScript_textChanged(const QString &arg1);

    void on_btAddSearchDepth_clicked();

    void on_twDepthSearch_cellDoubleClicked(int row, int column);

    void on_pbOpenDirectory_clicked();
    void on_PDBFinderButton_clicked();


    void on_runRMSD_clicked();

    void on_sbClusterSensitivity_textChanged(const QString &arg1);

    void on_sbClusterCount_textChanged(const QString &arg1);

    void on_cbConsolidateRes_stateChanged(int arg1);

    void on_sbLJThreshold_textChanged(const QString &arg1);

    void on_btExecuteStage0_clicked();

    void on_btInferSequence_clicked();

    void on_btRemoveFilter_clicked();

    void on_btAddFilter_clicked();

    void on_btStopStage0_clicked();

    void on_cbRamAngles_stateChanged(int arg1);

    void on_cbLoadFromTarball_stateChanged(int arg1);

    void on_btLoadFromTarball_clicked();

    void on_btGenerateREDCAT_clicked();

    void on_btClearRamAngles_clicked();

private:
    void findRDC();
    void inferSequence();
    void analyzeRDC();
    void refreshDirectory();
    void populateFileList();
    void executeStage1();
    void executeStage2();
    void openRMSD();
    void calculateResidues();
    void formatConfig();
    void readConfig();
    void writeConfig();
    void createNewConfig();
    void clearAngles();
    void clearRamAngles();
    void clearOutPDB();
    void convertRDC();
    void setStage1Elements(bool);
    void updateRecentsInMemory(const QString &recent);
    void updateRecentsView();
    bool openFolderDialog();
    bool openFolder(const QString &path);
    bool openREDCAT();
    bool openPDB();
    bool parseREDCATFile(QString redcat_file);
    bool parsePDBFile(QString pdb_file);


    Ui::MainWindow *ui;
    QString PYTHON_PATH = REDCRAFT_PATH + "redcraft/py/bin/python3";

    // General elements
    QTextBrowser *tbInfo;
    QListWidget *lwFileList;
    QTabWidget *tabWidget;
    QFileDialog dialog;
    QMenu *recentFolders;
    QStatusBar *statusBar;

    // Stage0 variables

    // pdbmine2.py
    QLineEdit *leTarballPath;
    QLineEdit *leResolution;
    QCheckBox *cbLoadFromTarball;
    QPushButton *btLoadFromTarball;
    QString TARBALL_PATH;

    // pdbmine.py
    QLineEdit *leURI;
    QLineEdit *leSequence;
    QPushButton *btInferSequence;
    QLineEdit *leWindowSize;
    QLineEdit *leClusterSize;
    QTextBrowser *tbStage0Output;
    QListWidget *lwFilterProteins;
    QPushButton *btAddFilter;
    QPushButton *btRemoveFilter;

    // Stage1 variables
    QString RDC_PREFIX = "RDC";
    QTextBrowser *tbStage1Output;
    QLineEdit *leRDC;
    QLineEdit *leCutoff;
    QCheckBox* cbRamAngles;
    QProcess *process;
    QProcess *process2;
    QProcess *process3;
    QProcess *process4;
    QSpinBox *sbMedia;
    QSpinBox *sbRLGly;
    QSpinBox *sbRLNonGly;
    int RAM_NONGLY = 1;
    int RAM_GLY = 0;
    double CUTOFF = -10000;
    bool RAMANGLES = false;

    // Stage2 basic variables
    QTextBrowser *tbStage2Output;
    QLineEdit *leSearchDepth;
    QCheckBox *cbRunType;
    QSpinBox *sbStartResidue;
    QSpinBox *sbStopResidue;
    int START_RESIDUES = 1;
    int STOP_RESIDUES = 10;
    int MEDIA_COUNT = 2;
    int SEARCH_DEPTH = 100;
    bool RUN_TYPE = 0;

    // Stage2 advanced variables
    QPushButton *btAddSearchDepth;
    QTableWidget *twDepthSearch;
    QLineEdit *dsbScoreThreshold;
    QLineEdit *sbClusterSensitivity;
    QLineEdit *sbLJThreshold;
    /*QSpinBox *sbDecimationStartResidue;
    QSpinBox *sbDecimationStopResidue;*/
    QLineEdit *leRefinementScript;
    DialogSearchDepth *dialogSearchDepth;
    QCheckBox *cbEnableDecimation;
    QCheckBox *cbEnableRefinement;
    bool ENABLE_SEARCH_DEPTH = 0;
    bool ENABLE_DECIMATION = 0;
    double LJ_THRESHOLD = 50.0;
    double DECIMATION_THRESHOLD = 2.5;
    int DECIMATION_CLUSTER_SENSITIVITY = 1;
    int DECIMATION_START_RESIDUE = 1;
    int DECIMATION_STOP_RESIDUE = 10;
    bool ENABLE_REFINEMENT = 0;
    QString REFINEMENT_SCRIPT = "unconstrained.prl";
    QString getDSSasString();

    // REDCAT variables
    QString REDCAT_FILE;
    QLabel *lbREDCATStatus;
    QPushButton *btFindREDCAT;
    QTableWidget *twREDCAT;
    QLineEdit *leREDCATOutput;
    QLineEdit *leSamplingsNull;
    QLineEdit *leSamplingsError;
    QLineEdit *leSearchRange;


    // OpenGL stuff
    PDBViewer *openGLWidget;
    QTabWidget *tabs;

    // Analysis suite
    AnalysisSuite *analysis;

    // RDC Profile Sidebar
    QDockWidget *dwRDCProfile;
    Histogram *histogram;
    QTextBrowser *tbRDCInfo;
    QCheckBox *cbConsolidateRes;


};

#endif // MAINWINDOW_H
