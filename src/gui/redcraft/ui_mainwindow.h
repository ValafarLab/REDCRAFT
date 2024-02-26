/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "histogram.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionProcess_RDC;
    QAction *actionGenerate_PDB;
    QAction *actionExit;
    QAction *actionOpen_Folder;
    QAction *actionAbout_REDCRAFT;
    QAction *actionREDCRAFT_Documentation;
    QAction *actionConvert_old_RDC_to_new;
    QAction *actionOpen_RMSD;
    QAction *actionl;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_13;
    QTabWidget *tabWidget;
    QWidget *tabStage0;
    QVBoxLayout *verticalLayout_12;
    QHBoxLayout *horizontalLayout_11;
    QFormLayout *formLayout_5;
    QCheckBox *cbLoadFromTarball;
    QLabel *label_24;
    QLineEdit *leSequence;
    QPushButton *btInferSequence;
    QLabel *label_23;
    QLineEdit *leURI;
    QLabel *label_25;
    QLineEdit *leWindowSize;
    QLabel *label_27;
    QHBoxLayout *hlFilterProteins;
    QListWidget *lwFilterProteins;
    QVBoxLayout *verticalLayout_3;
    QPushButton *btAddFilter;
    QPushButton *btRemoveFilter;
    QSpacerItem *verticalSpacer_3;
    QTextBrowser *tbStage0Output;
    QPushButton *btExecuteStage0;
    QPushButton *btStopStage0;
    QHBoxLayout *horizontalLayout_14;
    QLineEdit *leTarballPath;
    QPushButton *btLoadFromTarball;
    QLineEdit *leClusterSize;
    QLabel *label_26;
    QLabel *label_28;
    QLineEdit *leResolution;
    QPushButton *btClearRamAngles;
    QWidget *tabStage1;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QPushButton *btFindRDC;
    QLabel *label;
    QLineEdit *leRDC;
    QLabel *label_2;
    QSpinBox *sbRLNonGly;
    QLabel *label_3;
    QSpinBox *sbRLGly;
    QLabel *label_4;
    QLineEdit *leCutoff;
    QPushButton *btClearAngleFiles;
    QCheckBox *cbRamAngles;
    QTextBrowser *tbStage1Output;
    QPushButton *btRunStage1;
    QPushButton *btStopStage1;
    QWidget *tabStage2;
    QVBoxLayout *verticalLayout_5;
    QTabWidget *twStage2Settings;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_4;
    QFormLayout *formLayout_2;
    QCheckBox *cbRunType;
    QLabel *label_7;
    QSpinBox *sbStartResidue;
    QLabel *label_8;
    QSpinBox *sbStopResidue;
    QLabel *label_9;
    QSpinBox *sbMedia;
    QLabel *label_10;
    QLineEdit *leSearchDepth;
    QPushButton *btClearOutPDB;
    QPushButton *btUseAllResidues;
    QTextBrowser *tbStage2Output;
    QWidget *tab_5;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout;
    QPushButton *btAddSearchDepth;
    QTableWidget *twDepthSearch;
    QFormLayout *formLayout_3;
    QLabel *label_22;
    QLineEdit *sbLJThreshold;
    QCheckBox *cbEnableDecimation;
    QLabel *label_12;
    QLineEdit *sbClusterSensitivity;
    QLabel *label_13;
    QLineEdit *dsbScoreThreshold;
    QLabel *label_17;
    QLineEdit *DecimationRangelineEdit;
    QLabel *label_14;
    QLineEdit *sbClusterCount;
    QLabel *label_15;
    QLineEdit *sbMax;
    QSpacerItem *verticalSpacer;
    QCheckBox *cbEnableRefinement;
    QLabel *label_16;
    QLineEdit *leRefinementScript;
    QSpacerItem *verticalSpacer_2;
    QPushButton *btRunStage2;
    QPushButton *btStopStage2;
    QWidget *tabREDCAT;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_29;
    QPushButton *btGenerateREDCAT;
    QHBoxLayout *horizontalLayout_8;
    QLabel *lbREDCATStatus;
    QTableWidget *twREDCAT;
    QFormLayout *formLayout_4;
    QWidget *tabRMSD;
    QVBoxLayout *verticalLayout_11;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_6;
    QLineEdit *StartlineEdit;
    QLabel *label_11;
    QLineEdit *StoplineEdit;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *PDBFilelineEdit;
    QPushButton *PDBFinderButton;
    QPushButton *runRMSD;
    QTextBrowser *RMSDtextBrowser;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOpen_Recent;
    QMenu *menuHelp;
    QMenu *menuTools;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QTextBrowser *tbInfo;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btRefreshFiles;
    QPushButton *pbOpenDirectory;
    QListWidget *lwFileList;
    QDockWidget *dwRDCProfile;
    QWidget *dockWidgetContents_2;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_8;
    QTextBrowser *tbRDCInfo;
    QCheckBox *cbConsolidateRes;
    Histogram *histogram;
    QHBoxLayout *horizontalLayout_9;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1125, 649);
        QIcon icon;
        icon.addFile(QString::fromUtf8("redcraft_logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionProcess_RDC = new QAction(MainWindow);
        actionProcess_RDC->setObjectName(QString::fromUtf8("actionProcess_RDC"));
        actionGenerate_PDB = new QAction(MainWindow);
        actionGenerate_PDB->setObjectName(QString::fromUtf8("actionGenerate_PDB"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionOpen_Folder = new QAction(MainWindow);
        actionOpen_Folder->setObjectName(QString::fromUtf8("actionOpen_Folder"));
        actionAbout_REDCRAFT = new QAction(MainWindow);
        actionAbout_REDCRAFT->setObjectName(QString::fromUtf8("actionAbout_REDCRAFT"));
        actionREDCRAFT_Documentation = new QAction(MainWindow);
        actionREDCRAFT_Documentation->setObjectName(QString::fromUtf8("actionREDCRAFT_Documentation"));
        actionConvert_old_RDC_to_new = new QAction(MainWindow);
        actionConvert_old_RDC_to_new->setObjectName(QString::fromUtf8("actionConvert_old_RDC_to_new"));
        actionConvert_old_RDC_to_new->setEnabled(true);
        actionOpen_RMSD = new QAction(MainWindow);
        actionOpen_RMSD->setObjectName(QString::fromUtf8("actionOpen_RMSD"));
        actionl = new QAction(MainWindow);
        actionl->setObjectName(QString::fromUtf8("actionl"));
        actionl->setVisible(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_13 = new QHBoxLayout(centralWidget);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabStage0 = new QWidget();
        tabStage0->setObjectName(QString::fromUtf8("tabStage0"));
        verticalLayout_12 = new QVBoxLayout(tabStage0);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        formLayout_5 = new QFormLayout();
        formLayout_5->setSpacing(6);
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        formLayout_5->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        cbLoadFromTarball = new QCheckBox(tabStage0);
        cbLoadFromTarball->setObjectName(QString::fromUtf8("cbLoadFromTarball"));

        formLayout_5->setWidget(1, QFormLayout::LabelRole, cbLoadFromTarball);

        label_24 = new QLabel(tabStage0);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        formLayout_5->setWidget(4, QFormLayout::LabelRole, label_24);

        leSequence = new QLineEdit(tabStage0);
        leSequence->setObjectName(QString::fromUtf8("leSequence"));

        formLayout_5->setWidget(4, QFormLayout::FieldRole, leSequence);

        btInferSequence = new QPushButton(tabStage0);
        btInferSequence->setObjectName(QString::fromUtf8("btInferSequence"));

        formLayout_5->setWidget(5, QFormLayout::SpanningRole, btInferSequence);

        label_23 = new QLabel(tabStage0);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        formLayout_5->setWidget(7, QFormLayout::LabelRole, label_23);

        leURI = new QLineEdit(tabStage0);
        leURI->setObjectName(QString::fromUtf8("leURI"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leURI->sizePolicy().hasHeightForWidth());
        leURI->setSizePolicy(sizePolicy);

        formLayout_5->setWidget(7, QFormLayout::FieldRole, leURI);

        label_25 = new QLabel(tabStage0);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        formLayout_5->setWidget(12, QFormLayout::LabelRole, label_25);

        leWindowSize = new QLineEdit(tabStage0);
        leWindowSize->setObjectName(QString::fromUtf8("leWindowSize"));

        formLayout_5->setWidget(12, QFormLayout::FieldRole, leWindowSize);

        label_27 = new QLabel(tabStage0);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        formLayout_5->setWidget(15, QFormLayout::LabelRole, label_27);

        hlFilterProteins = new QHBoxLayout();
        hlFilterProteins->setSpacing(6);
        hlFilterProteins->setObjectName(QString::fromUtf8("hlFilterProteins"));
        hlFilterProteins->setContentsMargins(-1, -1, 0, -1);
        lwFilterProteins = new QListWidget(tabStage0);
        lwFilterProteins->setObjectName(QString::fromUtf8("lwFilterProteins"));
        QFont font;
        font.setPointSize(16);
        lwFilterProteins->setFont(font);

        hlFilterProteins->addWidget(lwFilterProteins);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, -1, 0, -1);
        btAddFilter = new QPushButton(tabStage0);
        btAddFilter->setObjectName(QString::fromUtf8("btAddFilter"));

        verticalLayout_3->addWidget(btAddFilter);

        btRemoveFilter = new QPushButton(tabStage0);
        btRemoveFilter->setObjectName(QString::fromUtf8("btRemoveFilter"));

        verticalLayout_3->addWidget(btRemoveFilter);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);


        hlFilterProteins->addLayout(verticalLayout_3);


        formLayout_5->setLayout(15, QFormLayout::FieldRole, hlFilterProteins);

        tbStage0Output = new QTextBrowser(tabStage0);
        tbStage0Output->setObjectName(QString::fromUtf8("tbStage0Output"));

        formLayout_5->setWidget(18, QFormLayout::SpanningRole, tbStage0Output);

        btExecuteStage0 = new QPushButton(tabStage0);
        btExecuteStage0->setObjectName(QString::fromUtf8("btExecuteStage0"));

        formLayout_5->setWidget(19, QFormLayout::SpanningRole, btExecuteStage0);

        btStopStage0 = new QPushButton(tabStage0);
        btStopStage0->setObjectName(QString::fromUtf8("btStopStage0"));
        btStopStage0->setStyleSheet(QString::fromUtf8("font-weight: bold;\n"
"color: rgb(170, 0, 0);"));

        formLayout_5->setWidget(20, QFormLayout::SpanningRole, btStopStage0);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        leTarballPath = new QLineEdit(tabStage0);
        leTarballPath->setObjectName(QString::fromUtf8("leTarballPath"));
        leTarballPath->setEnabled(false);

        horizontalLayout_14->addWidget(leTarballPath);

        btLoadFromTarball = new QPushButton(tabStage0);
        btLoadFromTarball->setObjectName(QString::fromUtf8("btLoadFromTarball"));
        btLoadFromTarball->setEnabled(false);

        horizontalLayout_14->addWidget(btLoadFromTarball);


        formLayout_5->setLayout(1, QFormLayout::FieldRole, horizontalLayout_14);

        leClusterSize = new QLineEdit(tabStage0);
        leClusterSize->setObjectName(QString::fromUtf8("leClusterSize"));

        formLayout_5->setWidget(13, QFormLayout::FieldRole, leClusterSize);

        label_26 = new QLabel(tabStage0);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        formLayout_5->setWidget(13, QFormLayout::LabelRole, label_26);

        label_28 = new QLabel(tabStage0);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        formLayout_5->setWidget(14, QFormLayout::LabelRole, label_28);

        leResolution = new QLineEdit(tabStage0);
        leResolution->setObjectName(QString::fromUtf8("leResolution"));
        leResolution->setEnabled(false);

        formLayout_5->setWidget(14, QFormLayout::FieldRole, leResolution);

        btClearRamAngles = new QPushButton(tabStage0);
        btClearRamAngles->setObjectName(QString::fromUtf8("btClearRamAngles"));

        formLayout_5->setWidget(17, QFormLayout::SpanningRole, btClearRamAngles);


        horizontalLayout_11->addLayout(formLayout_5);


        verticalLayout_12->addLayout(horizontalLayout_11);

        tabWidget->addTab(tabStage0, QString());
        tabStage1 = new QWidget();
        tabStage1->setObjectName(QString::fromUtf8("tabStage1"));
        horizontalLayout_6 = new QHBoxLayout(tabStage1);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        btFindRDC = new QPushButton(tabStage1);
        btFindRDC->setObjectName(QString::fromUtf8("btFindRDC"));

        formLayout->setWidget(0, QFormLayout::SpanningRole, btFindRDC);

        label = new QLabel(tabStage1);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        leRDC = new QLineEdit(tabStage1);
        leRDC->setObjectName(QString::fromUtf8("leRDC"));

        formLayout->setWidget(1, QFormLayout::FieldRole, leRDC);

        label_2 = new QLabel(tabStage1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_2);

        sbRLNonGly = new QSpinBox(tabStage1);
        sbRLNonGly->setObjectName(QString::fromUtf8("sbRLNonGly"));
        sbRLNonGly->setMinimum(1);
        sbRLNonGly->setMaximum(3);

        formLayout->setWidget(3, QFormLayout::FieldRole, sbRLNonGly);

        label_3 = new QLabel(tabStage1);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_3);

        sbRLGly = new QSpinBox(tabStage1);
        sbRLGly->setObjectName(QString::fromUtf8("sbRLGly"));
        sbRLGly->setMinimum(0);
        sbRLGly->setMaximum(2);
        sbRLGly->setValue(0);

        formLayout->setWidget(4, QFormLayout::FieldRole, sbRLGly);

        label_4 = new QLabel(tabStage1);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_4);

        leCutoff = new QLineEdit(tabStage1);
        leCutoff->setObjectName(QString::fromUtf8("leCutoff"));

        formLayout->setWidget(5, QFormLayout::FieldRole, leCutoff);

        btClearAngleFiles = new QPushButton(tabStage1);
        btClearAngleFiles->setObjectName(QString::fromUtf8("btClearAngleFiles"));

        formLayout->setWidget(7, QFormLayout::SpanningRole, btClearAngleFiles);

        cbRamAngles = new QCheckBox(tabStage1);
        cbRamAngles->setObjectName(QString::fromUtf8("cbRamAngles"));

        formLayout->setWidget(2, QFormLayout::SpanningRole, cbRamAngles);


        verticalLayout->addLayout(formLayout);

        tbStage1Output = new QTextBrowser(tabStage1);
        tbStage1Output->setObjectName(QString::fromUtf8("tbStage1Output"));

        verticalLayout->addWidget(tbStage1Output);

        btRunStage1 = new QPushButton(tabStage1);
        btRunStage1->setObjectName(QString::fromUtf8("btRunStage1"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        btRunStage1->setFont(font1);
        btRunStage1->setAutoFillBackground(false);
        btRunStage1->setStyleSheet(QString::fromUtf8(""));
        btRunStage1->setCheckable(false);

        verticalLayout->addWidget(btRunStage1);

        btStopStage1 = new QPushButton(tabStage1);
        btStopStage1->setObjectName(QString::fromUtf8("btStopStage1"));
        btStopStage1->setStyleSheet(QString::fromUtf8("font-weight: bold;\n"
"color: rgb(170, 0, 0);"));

        verticalLayout->addWidget(btStopStage1);


        horizontalLayout_6->addLayout(verticalLayout);

        tabWidget->addTab(tabStage1, QString());
        tabStage2 = new QWidget();
        tabStage2->setObjectName(QString::fromUtf8("tabStage2"));
        verticalLayout_5 = new QVBoxLayout(tabStage2);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        twStage2Settings = new QTabWidget(tabStage2);
        twStage2Settings->setObjectName(QString::fromUtf8("twStage2Settings"));
        twStage2Settings->setTabPosition(QTabWidget::East);
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_7 = new QVBoxLayout(tab_4);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        cbRunType = new QCheckBox(tab_4);
        cbRunType->setObjectName(QString::fromUtf8("cbRunType"));

        formLayout_2->setWidget(0, QFormLayout::SpanningRole, cbRunType);

        label_7 = new QLabel(tab_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_7);

        sbStartResidue = new QSpinBox(tab_4);
        sbStartResidue->setObjectName(QString::fromUtf8("sbStartResidue"));
        sizePolicy.setHeightForWidth(sbStartResidue->sizePolicy().hasHeightForWidth());
        sbStartResidue->setSizePolicy(sizePolicy);
        sbStartResidue->setMinimum(1);
        sbStartResidue->setMaximum(9999);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, sbStartResidue);

        label_8 = new QLabel(tab_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_8);

        sbStopResidue = new QSpinBox(tab_4);
        sbStopResidue->setObjectName(QString::fromUtf8("sbStopResidue"));
        sizePolicy.setHeightForWidth(sbStopResidue->sizePolicy().hasHeightForWidth());
        sbStopResidue->setSizePolicy(sizePolicy);
        sbStopResidue->setMaximum(9999);
        sbStopResidue->setValue(10);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, sbStopResidue);

        label_9 = new QLabel(tab_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_9);

        sbMedia = new QSpinBox(tab_4);
        sbMedia->setObjectName(QString::fromUtf8("sbMedia"));
        sizePolicy.setHeightForWidth(sbMedia->sizePolicy().hasHeightForWidth());
        sbMedia->setSizePolicy(sizePolicy);
        sbMedia->setValue(2);

        formLayout_2->setWidget(4, QFormLayout::FieldRole, sbMedia);

        label_10 = new QLabel(tab_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_10);

        leSearchDepth = new QLineEdit(tab_4);
        leSearchDepth->setObjectName(QString::fromUtf8("leSearchDepth"));

        formLayout_2->setWidget(5, QFormLayout::FieldRole, leSearchDepth);

        btClearOutPDB = new QPushButton(tab_4);
        btClearOutPDB->setObjectName(QString::fromUtf8("btClearOutPDB"));

        formLayout_2->setWidget(6, QFormLayout::SpanningRole, btClearOutPDB);

        btUseAllResidues = new QPushButton(tab_4);
        btUseAllResidues->setObjectName(QString::fromUtf8("btUseAllResidues"));

        formLayout_2->setWidget(3, QFormLayout::SpanningRole, btUseAllResidues);


        verticalLayout_4->addLayout(formLayout_2);


        verticalLayout_7->addLayout(verticalLayout_4);

        tbStage2Output = new QTextBrowser(tab_4);
        tbStage2Output->setObjectName(QString::fromUtf8("tbStage2Output"));

        verticalLayout_7->addWidget(tbStage2Output);

        twStage2Settings->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        verticalLayout_9 = new QVBoxLayout(tab_5);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        btAddSearchDepth = new QPushButton(tab_5);
        btAddSearchDepth->setObjectName(QString::fromUtf8("btAddSearchDepth"));
        btAddSearchDepth->setEnabled(true);

        horizontalLayout->addWidget(btAddSearchDepth);


        verticalLayout_9->addLayout(horizontalLayout);

        twDepthSearch = new QTableWidget(tab_5);
        if (twDepthSearch->columnCount() < 3)
            twDepthSearch->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        twDepthSearch->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        twDepthSearch->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        twDepthSearch->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        twDepthSearch->setObjectName(QString::fromUtf8("twDepthSearch"));
        twDepthSearch->setEnabled(true);
        twDepthSearch->setEditTriggers(QAbstractItemView::NoEditTriggers);
        twDepthSearch->setAlternatingRowColors(true);

        verticalLayout_9->addWidget(twDepthSearch);

        formLayout_3 = new QFormLayout();
        formLayout_3->setSpacing(6);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        formLayout_3->setContentsMargins(-1, 0, -1, -1);
        label_22 = new QLabel(tab_5);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_22);

        sbLJThreshold = new QLineEdit(tab_5);
        sbLJThreshold->setObjectName(QString::fromUtf8("sbLJThreshold"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, sbLJThreshold);

        cbEnableDecimation = new QCheckBox(tab_5);
        cbEnableDecimation->setObjectName(QString::fromUtf8("cbEnableDecimation"));
        cbEnableDecimation->setEnabled(true);

        formLayout_3->setWidget(2, QFormLayout::LabelRole, cbEnableDecimation);

        label_12 = new QLabel(tab_5);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, label_12);

        sbClusterSensitivity = new QLineEdit(tab_5);
        sbClusterSensitivity->setObjectName(QString::fromUtf8("sbClusterSensitivity"));
        sbClusterSensitivity->setEnabled(false);

        formLayout_3->setWidget(3, QFormLayout::FieldRole, sbClusterSensitivity);

        label_13 = new QLabel(tab_5);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        formLayout_3->setWidget(4, QFormLayout::LabelRole, label_13);

        dsbScoreThreshold = new QLineEdit(tab_5);
        dsbScoreThreshold->setObjectName(QString::fromUtf8("dsbScoreThreshold"));
        dsbScoreThreshold->setEnabled(false);

        formLayout_3->setWidget(4, QFormLayout::FieldRole, dsbScoreThreshold);

        label_17 = new QLabel(tab_5);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        formLayout_3->setWidget(5, QFormLayout::LabelRole, label_17);

        DecimationRangelineEdit = new QLineEdit(tab_5);
        DecimationRangelineEdit->setObjectName(QString::fromUtf8("DecimationRangelineEdit"));
        DecimationRangelineEdit->setEnabled(false);

        formLayout_3->setWidget(5, QFormLayout::FieldRole, DecimationRangelineEdit);

        label_14 = new QLabel(tab_5);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        formLayout_3->setWidget(6, QFormLayout::LabelRole, label_14);

        sbClusterCount = new QLineEdit(tab_5);
        sbClusterCount->setObjectName(QString::fromUtf8("sbClusterCount"));
        sbClusterCount->setEnabled(false);

        formLayout_3->setWidget(6, QFormLayout::FieldRole, sbClusterCount);

        label_15 = new QLabel(tab_5);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        formLayout_3->setWidget(7, QFormLayout::LabelRole, label_15);

        sbMax = new QLineEdit(tab_5);
        sbMax->setObjectName(QString::fromUtf8("sbMax"));
        sbMax->setEnabled(false);

        formLayout_3->setWidget(7, QFormLayout::FieldRole, sbMax);

        verticalSpacer = new QSpacerItem(20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout_3->setItem(9, QFormLayout::SpanningRole, verticalSpacer);

        cbEnableRefinement = new QCheckBox(tab_5);
        cbEnableRefinement->setObjectName(QString::fromUtf8("cbEnableRefinement"));

        formLayout_3->setWidget(10, QFormLayout::LabelRole, cbEnableRefinement);

        label_16 = new QLabel(tab_5);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        formLayout_3->setWidget(11, QFormLayout::LabelRole, label_16);

        leRefinementScript = new QLineEdit(tab_5);
        leRefinementScript->setObjectName(QString::fromUtf8("leRefinementScript"));
        leRefinementScript->setEnabled(false);
        sizePolicy.setHeightForWidth(leRefinementScript->sizePolicy().hasHeightForWidth());
        leRefinementScript->setSizePolicy(sizePolicy);

        formLayout_3->setWidget(11, QFormLayout::FieldRole, leRefinementScript);

        verticalSpacer_2 = new QSpacerItem(20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout_3->setItem(1, QFormLayout::LabelRole, verticalSpacer_2);


        verticalLayout_9->addLayout(formLayout_3);

        twStage2Settings->addTab(tab_5, QString());

        verticalLayout_5->addWidget(twStage2Settings);

        btRunStage2 = new QPushButton(tabStage2);
        btRunStage2->setObjectName(QString::fromUtf8("btRunStage2"));
        btRunStage2->setFont(font1);
        btRunStage2->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_5->addWidget(btRunStage2);

        btStopStage2 = new QPushButton(tabStage2);
        btStopStage2->setObjectName(QString::fromUtf8("btStopStage2"));
        btStopStage2->setStyleSheet(QString::fromUtf8("font-weight: bold;\n"
"color: rgb(170, 0, 0);"));

        verticalLayout_5->addWidget(btStopStage2);

        tabWidget->addTab(tabStage2, QString());
        tabREDCAT = new QWidget();
        tabREDCAT->setObjectName(QString::fromUtf8("tabREDCAT"));
        horizontalLayout_7 = new QHBoxLayout(tabREDCAT);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(-1, 5, -1, -1);
        label_29 = new QLabel(tabREDCAT);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        horizontalLayout_12->addWidget(label_29);

        btGenerateREDCAT = new QPushButton(tabREDCAT);
        btGenerateREDCAT->setObjectName(QString::fromUtf8("btGenerateREDCAT"));

        horizontalLayout_12->addWidget(btGenerateREDCAT);


        verticalLayout_6->addLayout(horizontalLayout_12);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(-1, 0, -1, -1);
        lbREDCATStatus = new QLabel(tabREDCAT);
        lbREDCATStatus->setObjectName(QString::fromUtf8("lbREDCATStatus"));

        horizontalLayout_8->addWidget(lbREDCATStatus);


        verticalLayout_6->addLayout(horizontalLayout_8);

        twREDCAT = new QTableWidget(tabREDCAT);
        if (twREDCAT->columnCount() < 10)
            twREDCAT->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        twREDCAT->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        twREDCAT->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        twREDCAT->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        twREDCAT->setHorizontalHeaderItem(3, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        twREDCAT->setHorizontalHeaderItem(4, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        twREDCAT->setHorizontalHeaderItem(5, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        twREDCAT->setHorizontalHeaderItem(6, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        twREDCAT->setHorizontalHeaderItem(7, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        twREDCAT->setHorizontalHeaderItem(8, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        twREDCAT->setHorizontalHeaderItem(9, __qtablewidgetitem12);
        if (twREDCAT->rowCount() < 50)
            twREDCAT->setRowCount(50);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        twREDCAT->setItem(0, 0, __qtablewidgetitem13);
        twREDCAT->setObjectName(QString::fromUtf8("twREDCAT"));
        twREDCAT->setEnabled(false);
        twREDCAT->setRowCount(50);
        twREDCAT->setColumnCount(10);
        twREDCAT->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_6->addWidget(twREDCAT);

        formLayout_4 = new QFormLayout();
        formLayout_4->setSpacing(6);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setContentsMargins(-1, 0, 0, -1);

        verticalLayout_6->addLayout(formLayout_4);


        horizontalLayout_7->addLayout(verticalLayout_6);

        tabWidget->addTab(tabREDCAT, QString());
        tabRMSD = new QWidget();
        tabRMSD->setObjectName(QString::fromUtf8("tabRMSD"));
        verticalLayout_11 = new QVBoxLayout(tabRMSD);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_6 = new QLabel(tabRMSD);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_3->addWidget(label_6);

        StartlineEdit = new QLineEdit(tabRMSD);
        StartlineEdit->setObjectName(QString::fromUtf8("StartlineEdit"));

        horizontalLayout_3->addWidget(StartlineEdit);

        label_11 = new QLabel(tabRMSD);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_3->addWidget(label_11);

        StoplineEdit = new QLineEdit(tabRMSD);
        StoplineEdit->setObjectName(QString::fromUtf8("StoplineEdit"));

        horizontalLayout_3->addWidget(StoplineEdit);


        verticalLayout_10->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        PDBFilelineEdit = new QLineEdit(tabRMSD);
        PDBFilelineEdit->setObjectName(QString::fromUtf8("PDBFilelineEdit"));

        horizontalLayout_4->addWidget(PDBFilelineEdit);

        PDBFinderButton = new QPushButton(tabRMSD);
        PDBFinderButton->setObjectName(QString::fromUtf8("PDBFinderButton"));

        horizontalLayout_4->addWidget(PDBFinderButton);


        verticalLayout_10->addLayout(horizontalLayout_4);

        runRMSD = new QPushButton(tabRMSD);
        runRMSD->setObjectName(QString::fromUtf8("runRMSD"));
        runRMSD->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(x1: 0.5, y1: 0.5 x2: 0.5, y2: 1, stop: 0 #4cbdff, stop: 0.5 #33a4e8);\n"
"    color: white;"));

        verticalLayout_10->addWidget(runRMSD);

        RMSDtextBrowser = new QTextBrowser(tabRMSD);
        RMSDtextBrowser->setObjectName(QString::fromUtf8("RMSDtextBrowser"));

        verticalLayout_10->addWidget(RMSDtextBrowser);


        verticalLayout_11->addLayout(verticalLayout_10);

        tabWidget->addTab(tabRMSD, QString());

        horizontalLayout_13->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1125, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFile->setGeometry(QRect(396, 108, 177, 196));
        menuOpen_Recent = new QMenu(menuFile);
        menuOpen_Recent->setObjectName(QString::fromUtf8("menuOpen_Recent"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(336, 274));
        dockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        horizontalLayout_5 = new QHBoxLayout(dockWidgetContents);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, -1, -1);
        tbInfo = new QTextBrowser(dockWidgetContents);
        tbInfo->setObjectName(QString::fromUtf8("tbInfo"));
        QFont font2;
        font2.setPointSize(12);
        tbInfo->setFont(font2);
        tbInfo->setFrameShape(QFrame::Box);
        tbInfo->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(tbInfo);

        label_5 = new QLabel(dockWidgetContents);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_2->addWidget(label_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, -1);
        btRefreshFiles = new QPushButton(dockWidgetContents);
        btRefreshFiles->setObjectName(QString::fromUtf8("btRefreshFiles"));

        horizontalLayout_2->addWidget(btRefreshFiles);

        pbOpenDirectory = new QPushButton(dockWidgetContents);
        pbOpenDirectory->setObjectName(QString::fromUtf8("pbOpenDirectory"));

        horizontalLayout_2->addWidget(pbOpenDirectory);


        verticalLayout_2->addLayout(horizontalLayout_2);

        lwFileList = new QListWidget(dockWidgetContents);
        lwFileList->setObjectName(QString::fromUtf8("lwFileList"));
        lwFileList->setSortingEnabled(false);

        verticalLayout_2->addWidget(lwFileList);


        horizontalLayout_5->addLayout(verticalLayout_2);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
        dwRDCProfile = new QDockWidget(MainWindow);
        dwRDCProfile->setObjectName(QString::fromUtf8("dwRDCProfile"));
        dwRDCProfile->setEnabled(true);
        dwRDCProfile->setMinimumSize(QSize(275, 419));
        dwRDCProfile->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        horizontalLayout_10 = new QHBoxLayout(dockWidgetContents_2);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, 0, -1, -1);
        tbRDCInfo = new QTextBrowser(dockWidgetContents_2);
        tbRDCInfo->setObjectName(QString::fromUtf8("tbRDCInfo"));
        tbRDCInfo->setFont(font2);
        tbRDCInfo->setFrameShape(QFrame::Box);
        tbRDCInfo->setFrameShadow(QFrame::Raised);

        verticalLayout_8->addWidget(tbRDCInfo);

        cbConsolidateRes = new QCheckBox(dockWidgetContents_2);
        cbConsolidateRes->setObjectName(QString::fromUtf8("cbConsolidateRes"));
        cbConsolidateRes->setChecked(false);

        verticalLayout_8->addWidget(cbConsolidateRes);

        histogram = new Histogram(dockWidgetContents_2);
        histogram->setObjectName(QString::fromUtf8("histogram"));
        histogram->setMinimumSize(QSize(0, 250));
        histogram->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_8->addWidget(histogram);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(-1, 0, -1, -1);

        verticalLayout_8->addLayout(horizontalLayout_9);


        horizontalLayout_10->addLayout(verticalLayout_8);

        dwRDCProfile->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dwRDCProfile);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen_Folder);
        menuFile->addAction(actionOpen_RMSD);
        menuFile->addAction(menuOpen_Recent->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionProcess_RDC);
        menuFile->addAction(actionGenerate_PDB);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuOpen_Recent->addAction(actionl);
        menuHelp->addAction(actionREDCRAFT_Documentation);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout_REDCRAFT);
        menuTools->addAction(actionConvert_old_RDC_to_new);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);
        twStage2Settings->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "REDCRAFT", nullptr));
        actionProcess_RDC->setText(QCoreApplication::translate("MainWindow", "Process RDC", nullptr));
#if QT_CONFIG(tooltip)
        actionProcess_RDC->setToolTip(QCoreApplication::translate("MainWindow", "Process RDC files (stage 1)", nullptr));
#endif // QT_CONFIG(tooltip)
        actionGenerate_PDB->setText(QCoreApplication::translate("MainWindow", "Generate PDB", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionOpen_Folder->setText(QCoreApplication::translate("MainWindow", "Open Workspace...", nullptr));
        actionAbout_REDCRAFT->setText(QCoreApplication::translate("MainWindow", "About REDCRAFT", nullptr));
        actionREDCRAFT_Documentation->setText(QCoreApplication::translate("MainWindow", "REDCRAFT Documentation", nullptr));
        actionConvert_old_RDC_to_new->setText(QCoreApplication::translate("MainWindow", "Convert old RDC to new", nullptr));
        actionOpen_RMSD->setText(QCoreApplication::translate("MainWindow", "Open RMSD", nullptr));
        actionl->setText(QCoreApplication::translate("MainWindow", "l", nullptr));
        cbLoadFromTarball->setText(QCoreApplication::translate("MainWindow", "Load From Tarball", nullptr));
#if QT_CONFIG(tooltip)
        label_24->setToolTip(QCoreApplication::translate("MainWindow", "Amino acid sequence in single character format.", nullptr));
#endif // QT_CONFIG(tooltip)
        label_24->setText(QCoreApplication::translate("MainWindow", "Sequence", nullptr));
        btInferSequence->setText(QCoreApplication::translate("MainWindow", "Infer Sequence from RDC", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "PDBMine URI", nullptr));
        leURI->setText(QCoreApplication::translate("MainWindow", "https://ifestos.cse.sc.edu", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "Window Size", nullptr));
        leWindowSize->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "Filter Proteins", nullptr));
        btAddFilter->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        btRemoveFilter->setText(QCoreApplication::translate("MainWindow", "Remove", nullptr));
        btExecuteStage0->setText(QCoreApplication::translate("MainWindow", "Execute Stage 0", nullptr));
        btStopStage0->setText(QCoreApplication::translate("MainWindow", "Stop Stage 0", nullptr));
        btLoadFromTarball->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        leClusterSize->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "Cluster Size", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "Resolution", nullptr));
        leResolution->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        btClearRamAngles->setText(QCoreApplication::translate("MainWindow", "Clear .ramangles files", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabStage0), QCoreApplication::translate("MainWindow", "S0: Infer Constraints", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setTabToolTip(tabWidget->indexOf(tabStage0), QCoreApplication::translate("MainWindow", "Uses PDBMine to intelligently constrain possible angles based off of real proteins from an online database.", nullptr));
#endif // QT_CONFIG(tooltip)
        btFindRDC->setText(QCoreApplication::translate("MainWindow", "Attempt to find RDC Prefix", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "RDC Prefix", nullptr));
        leRDC->setText(QCoreApplication::translate("MainWindow", "RDC", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Ramachandran Level (non-GLY)", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Ramachandran Level (GLY)", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Cutoff (decimal value in Hz)", nullptr));
        leCutoff->setText(QCoreApplication::translate("MainWindow", "none", nullptr));
        btClearAngleFiles->setText(QCoreApplication::translate("MainWindow", "Clear .angles files", nullptr));
        cbRamAngles->setText(QCoreApplication::translate("MainWindow", "Use custom .ramangles generated from stage 0", nullptr));
        btRunStage1->setText(QCoreApplication::translate("MainWindow", "Execute Stage 1", nullptr));
        btStopStage1->setText(QCoreApplication::translate("MainWindow", "Stop Stage 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabStage1), QCoreApplication::translate("MainWindow", "S1: Generate Angles", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setTabToolTip(tabWidget->indexOf(tabStage1), QCoreApplication::translate("MainWindow", "Generate .angles files that contain combinations of possible phi/psi angles for any given residue N from 1 to the length of the polypeptide.", nullptr));
#endif // QT_CONFIG(tooltip)
        cbRunType->setText(QCoreApplication::translate("MainWindow", "Continue from existing .out files", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Start Residue", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Stop Residue", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Media Volumes", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Search Depth", nullptr));
        leSearchDepth->setText(QCoreApplication::translate("MainWindow", "100", nullptr));
        leSearchDepth->setPlaceholderText(QCoreApplication::translate("MainWindow", "100", nullptr));
        btClearOutPDB->setText(QCoreApplication::translate("MainWindow", "Clear .out, .redcat, and .pdb files", nullptr));
        btUseAllResidues->setText(QCoreApplication::translate("MainWindow", "Use all residues", nullptr));
        twStage2Settings->setTabText(twStage2Settings->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Basic", nullptr));
        btAddSearchDepth->setText(QCoreApplication::translate("MainWindow", "Add Search Depth", nullptr));
        QTableWidgetItem *___qtablewidgetitem = twDepthSearch->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Enabled", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = twDepthSearch->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Residue", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = twDepthSearch->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Search Depth", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "Van Der Waals (LJ) Threshold", nullptr));
        sbLJThreshold->setText(QCoreApplication::translate("MainWindow", "50.0", nullptr));
        cbEnableDecimation->setText(QCoreApplication::translate("MainWindow", "Enable decimation", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Cluster Sensitivity", nullptr));
#if QT_CONFIG(tooltip)
        sbClusterSensitivity->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>This is the number of structures in each cluster. Cannot be used with Cluster Count</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        sbClusterSensitivity->setText(QString());
        sbClusterSensitivity->setPlaceholderText(QCoreApplication::translate("MainWindow", "1.00", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Score Threshold", nullptr));
#if QT_CONFIG(tooltip)
        dsbScoreThreshold->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>The value entered here is a percent. REDCRAFT then takes that percent and uses it to calculate the exact score threshold by calculating X*(1+percent) where X is the previous worst structure considered and percent is the value entered in the config file. </p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        dsbScoreThreshold->setText(QString());
        dsbScoreThreshold->setPlaceholderText(QCoreApplication::translate("MainWindow", "1.00", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Decimation Range", nullptr));
#if QT_CONFIG(tooltip)
        DecimationRangelineEdit->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Sepereate ranges with a comma. Use a dash to include residues inbetween. 4-6 is the same as 4,5,6</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        DecimationRangelineEdit->setText(QString());
        DecimationRangelineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "3, 4-6, 9", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Cluster Count", nullptr));
#if QT_CONFIG(tooltip)
        sbClusterCount->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>This is the approximate number of total cluster that will be created. Cannot be used with Cluster Sensitivity</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        sbClusterCount->setText(QString());
        sbClusterCount->setPlaceholderText(QCoreApplication::translate("MainWindow", "3000", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Max Number of Added Structures", nullptr));
#if QT_CONFIG(tooltip)
        sbMax->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>This is the maximum number of structures that can be added to the .out files. This value can be blank</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        sbMax->setText(QString());
        sbMax->setPlaceholderText(QCoreApplication::translate("MainWindow", "100", nullptr));
        cbEnableRefinement->setText(QCoreApplication::translate("MainWindow", "Enable Refinement", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Refinement Script", nullptr));
        leRefinementScript->setText(QCoreApplication::translate("MainWindow", "unconstrained.prl", nullptr));
        twStage2Settings->setTabText(twStage2Settings->indexOf(tab_5), QCoreApplication::translate("MainWindow", "Advanced", nullptr));
        btRunStage2->setText(QCoreApplication::translate("MainWindow", "Execute Stage 2", nullptr));
        btStopStage2->setText(QCoreApplication::translate("MainWindow", "Stop Stage 2", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabStage2), QCoreApplication::translate("MainWindow", "S2: Generate Structure", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setTabToolTip(tabWidget->indexOf(tabStage2), QCoreApplication::translate("MainWindow", "Generate a PDB structure given RDC data and .angles files generated from Stage 1.", nullptr));
#endif // QT_CONFIG(tooltip)
        label_29->setText(QCoreApplication::translate("MainWindow", "Generate RDCs from PDB File", nullptr));
        btGenerateREDCAT->setText(QCoreApplication::translate("MainWindow", "Select .pdb file", nullptr));
#if QT_CONFIG(tooltip)
        lbREDCATStatus->setToolTip(QCoreApplication::translate("MainWindow", "Select a .redcat file manually or double click a .redcat file in the file browser to begin analysis.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbREDCATStatus->setText(QCoreApplication::translate("MainWindow", "No REDCAT PDB file currently selected for analysis.", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = twREDCAT->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Eq.#", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = twREDCAT->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "X1", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = twREDCAT->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Y1", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = twREDCAT->horizontalHeaderItem(3);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Z1", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = twREDCAT->horizontalHeaderItem(4);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "X2", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = twREDCAT->horizontalHeaderItem(5);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Y2", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = twREDCAT->horizontalHeaderItem(6);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Z2", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = twREDCAT->horizontalHeaderItem(7);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Dipol 1", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = twREDCAT->horizontalHeaderItem(8);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "Error", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = twREDCAT->horizontalHeaderItem(9);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "Comments", nullptr));

        const bool __sortingEnabled = twREDCAT->isSortingEnabled();
        twREDCAT->setSortingEnabled(false);
        twREDCAT->setSortingEnabled(__sortingEnabled);

        tabWidget->setTabText(tabWidget->indexOf(tabREDCAT), QCoreApplication::translate("MainWindow", "PDB -> RDC", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setTabToolTip(tabWidget->indexOf(tabREDCAT), QCoreApplication::translate("MainWindow", "Use REDCAT to generate simulated RDC data from a given PDB file.", nullptr));
#endif // QT_CONFIG(tooltip)
        label_6->setText(QCoreApplication::translate("MainWindow", "Start Residue", nullptr));
#if QT_CONFIG(tooltip)
        StartlineEdit->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Typlically 2 is the starting residue because REDCRAFT does not generate a 1.pdb file unless ran in reverse</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        StartlineEdit->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Stop Residue", nullptr));
        PDBFinderButton->setText(QCoreApplication::translate("MainWindow", "Find PDB", nullptr));
        runRMSD->setText(QCoreApplication::translate("MainWindow", "Calculate RMSD", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabRMSD), QCoreApplication::translate("MainWindow", "RMSD", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuOpen_Recent->setTitle(QCoreApplication::translate("MainWindow", "Open Recent...", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "Tools", nullptr));
#if QT_CONFIG(statustip)
        statusBar->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        dockWidget->setWindowTitle(QCoreApplication::translate("MainWindow", "REDCRAFT File Browser", nullptr));
        tbInfo->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'.AppleSystemUIFont'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Cantarell';\"><br /></p></body></html>", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "File Browser", nullptr));
        btRefreshFiles->setText(QCoreApplication::translate("MainWindow", "Refresh file list", nullptr));
        pbOpenDirectory->setText(QCoreApplication::translate("MainWindow", "Open Active Directory", nullptr));
        dwRDCProfile->setWindowTitle(QCoreApplication::translate("MainWindow", "RDC Profile", nullptr));
        tbRDCInfo->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'.AppleSystemUIFont'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Cantarell';\"><br /></p></body></html>", nullptr));
        cbConsolidateRes->setText(QCoreApplication::translate("MainWindow", "Consolidate Residues", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
