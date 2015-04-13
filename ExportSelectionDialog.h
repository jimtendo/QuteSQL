#ifndef EXPORTSELECTIONDIALOG_H
#define EXPORTSELECTIONDIALOG_H

#include <QDialog>
#include <QTableView>

namespace Ui {
class ExportSelectionDialog;
}

class ExportSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportSelectionDialog(QWidget *parent = 0);
    ~ExportSelectionDialog();

    void init(QTableView *tableView);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ExportSelectionDialog *ui;

    QTableView *m_tableView;
};

#endif // EXPORTSELECTIONDIALOG_H
