/********************************************************************************
** Form generated from reading UI file 'dialogsearchdepth.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSEARCHDEPTH_H
#define UI_DIALOGSEARCHDEPTH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_DialogSearchDepth
{
public:
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout_2;
    QLabel *label;
    QSpinBox *sbResidue;
    QLabel *label_2;
    QLineEdit *leSearchDepth;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogSearchDepth)
    {
        if (DialogSearchDepth->objectName().isEmpty())
            DialogSearchDepth->setObjectName(QString::fromUtf8("DialogSearchDepth"));
        DialogSearchDepth->resize(400, 84);
        horizontalLayout = new QHBoxLayout(DialogSearchDepth);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label = new QLabel(DialogSearchDepth);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label);

        sbResidue = new QSpinBox(DialogSearchDepth);
        sbResidue->setObjectName(QString::fromUtf8("sbResidue"));
        sbResidue->setMinimum(1);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, sbResidue);

        label_2 = new QLabel(DialogSearchDepth);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_2);

        leSearchDepth = new QLineEdit(DialogSearchDepth);
        leSearchDepth->setObjectName(QString::fromUtf8("leSearchDepth"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, leSearchDepth);


        horizontalLayout->addLayout(formLayout_2);

        buttonBox = new QDialogButtonBox(DialogSearchDepth);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);


        retranslateUi(DialogSearchDepth);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogSearchDepth, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogSearchDepth, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogSearchDepth);
    } // setupUi

    void retranslateUi(QDialog *DialogSearchDepth)
    {
        DialogSearchDepth->setWindowTitle(QCoreApplication::translate("DialogSearchDepth", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("DialogSearchDepth", "Residue", nullptr));
        label_2->setText(QCoreApplication::translate("DialogSearchDepth", "Search Depth", nullptr));
        leSearchDepth->setText(QCoreApplication::translate("DialogSearchDepth", "1000", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogSearchDepth: public Ui_DialogSearchDepth {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSEARCHDEPTH_H
