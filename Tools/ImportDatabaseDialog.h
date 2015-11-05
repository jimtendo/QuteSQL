#ifndef IMPORTDATABASEDIALOG_H
#define IMPORTDATABASEDIALOG_H

#include <QDialog>

namespace Ui {
class ImportDatabaseDialog;
}

class ImportDatabaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDatabaseDialog(QWidget *parent = 0);
    ~ImportDatabaseDialog();

private:
    Ui::ImportDatabaseDialog *ui;
};

#endif // IMPORTDATABASEDIALOG_H
