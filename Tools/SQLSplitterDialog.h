#ifndef SQLSPLITTERDIALOG_H
#define SQLSPLITTERDIALOG_H

#include <QDialog>

namespace Ui {
class SQLSplitterDialog;
}

class SQLSplitterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SQLSplitterDialog(QWidget *parent = 0);
    ~SQLSplitterDialog();

private slots:
    void on_inputFileButton_clicked();

    void on_outputFileButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::SQLSplitterDialog *ui;
};

#endif // SQLSPLITTERDIALOG_H
