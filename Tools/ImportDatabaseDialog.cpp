#include "ImportDatabaseDialog.h"
#include "ui_ImportDatabaseDialog.h"

ImportDatabaseDialog::ImportDatabaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDatabaseDialog)
{
    ui->setupUi(this);
}

ImportDatabaseDialog::~ImportDatabaseDialog()
{
    delete ui;
}
