#ifndef DIALOGSEARCHDEPTH_H
#define DIALOGSEARCHDEPTH_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>

namespace Ui {
class DialogSearchDepth;
}

class DialogSearchDepth : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSearchDepth(QTableWidget *table, QWidget *parent = nullptr);
    ~DialogSearchDepth();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogSearchDepth *ui;
    QLineEdit *leSearchDepth;
    QSpinBox *sbResidue;
    QTableWidget *table;
};

#endif // DIALOGSEARCHDEPTH_H
