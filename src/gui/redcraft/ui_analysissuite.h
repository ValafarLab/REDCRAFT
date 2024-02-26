/********************************************************************************
** Form generated from reading UI file 'analysissuite.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANALYSISSUITE_H
#define UI_ANALYSISSUITE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "dynamicprofile.h"

QT_BEGIN_NAMESPACE

class Ui_AnalysisSuite
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QTableWidget *twDataView;
    QMenuBar *menubar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QListWidget *lwInfoBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpinBox *sbStartResidue;
    QLabel *label_2;
    QSpinBox *sbEndResidue;
    DynamicProfile *dynamicprofile;

    void setupUi(QMainWindow *AnalysisSuite)
    {
        if (AnalysisSuite->objectName().isEmpty())
            AnalysisSuite->setObjectName(QString::fromUtf8("AnalysisSuite"));
        AnalysisSuite->resize(1103, 706);
        centralwidget = new QWidget(AnalysisSuite);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, 0, -1);
        twDataView = new QTableWidget(centralwidget);
        twDataView->setObjectName(QString::fromUtf8("twDataView"));
        twDataView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(twDataView);


        horizontalLayout->addLayout(verticalLayout);

        AnalysisSuite->setCentralWidget(centralwidget);
        menubar = new QMenuBar(AnalysisSuite);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1103, 21));
        AnalysisSuite->setMenuBar(menubar);
        dockWidget = new QDockWidget(AnalysisSuite);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(320, 466));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lwInfoBox = new QListWidget(dockWidgetContents);
        lwInfoBox->setObjectName(QString::fromUtf8("lwInfoBox"));
        lwInfoBox->setMaximumSize(QSize(500, 16777215));

        verticalLayout_2->addWidget(lwInfoBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 20, -1, -1);
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        sbStartResidue = new QSpinBox(dockWidgetContents);
        sbStartResidue->setObjectName(QString::fromUtf8("sbStartResidue"));
        sbStartResidue->setMinimum(2);
        sbStartResidue->setMaximum(999999999);

        horizontalLayout_2->addWidget(sbStartResidue);

        label_2 = new QLabel(dockWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        sbEndResidue = new QSpinBox(dockWidgetContents);
        sbEndResidue->setObjectName(QString::fromUtf8("sbEndResidue"));
        sbEndResidue->setMinimum(2);
        sbEndResidue->setMaximum(999999999);

        horizontalLayout_2->addWidget(sbEndResidue);


        verticalLayout_2->addLayout(horizontalLayout_2);

        dynamicprofile = new DynamicProfile(dockWidgetContents);
        dynamicprofile->setObjectName(QString::fromUtf8("dynamicprofile"));
        dynamicprofile->setMinimumSize(QSize(275, 300));
        dynamicprofile->setMaximumSize(QSize(500, 16777215));

        verticalLayout_2->addWidget(dynamicprofile);

        dockWidget->setWidget(dockWidgetContents);
        AnalysisSuite->addDockWidget(Qt::RightDockWidgetArea, dockWidget);

        retranslateUi(AnalysisSuite);

        QMetaObject::connectSlotsByName(AnalysisSuite);
    } // setupUi

    void retranslateUi(QMainWindow *AnalysisSuite)
    {
        AnalysisSuite->setWindowTitle(QCoreApplication::translate("AnalysisSuite", "REDCRAFT - Analysis Suite", nullptr));
        dockWidget->setWindowTitle(QCoreApplication::translate("AnalysisSuite", "Dynamic Profile", nullptr));
        label->setText(QCoreApplication::translate("AnalysisSuite", "Start Residue:", nullptr));
        label_2->setText(QCoreApplication::translate("AnalysisSuite", "End Residue:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AnalysisSuite: public Ui_AnalysisSuite {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANALYSISSUITE_H
