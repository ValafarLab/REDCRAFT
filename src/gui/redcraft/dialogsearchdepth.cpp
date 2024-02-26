#include "dialogsearchdepth.h"
#include "ui_dialogsearchdepth.h"

DialogSearchDepth::DialogSearchDepth(QTableWidget *table, QWidget *parent) :
    QDialog(parent),
    table(table),
    ui(new Ui::DialogSearchDepth)
{
    ui->setupUi(this);
    leSearchDepth = this->findChild<QLineEdit *>("leSearchDepth");
    sbResidue = this->findChild<QSpinBox *>("sbResidue");
    leSearchDepth->setValidator( new QIntValidator(0, 5000, this) );
//    this->table = table;
}

DialogSearchDepth::~DialogSearchDepth()
{
    delete ui;
}

void DialogSearchDepth::on_buttonBox_accepted()
{
    if (table != nullptr) {
        // Find place to put new content
        for (int i = 0; i < table->rowCount(); i++) {
            if (table->item(i,1)->text() == sbResidue->text()) {
                // We have found the same item here
                table->item(i,2)->setText(leSearchDepth->text());
                return;
            }
        }
        // Here it must be a new one
        QTableWidgetItem *enabled = new QTableWidgetItem("Enabled");
        enabled->setCheckState(Qt::CheckState::Checked);
        QTableWidgetItem *residue = new QTableWidgetItem(sbResidue->text());
        QTableWidgetItem *depth = new QTableWidgetItem(leSearchDepth->text());
        int row = table->rowCount();
        table->insertRow(row);
        int col = table->columnCount();
        if (col == 0) {
            table->insertColumn(0);
            table->insertColumn(1);
            table->insertColumn(2);
        }
        table->setItem(row, 0, enabled);
        table->setItem(row, 1, residue);
        table->setItem(row, 2, depth);

    }
}
