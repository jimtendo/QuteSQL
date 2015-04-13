#include "ExportSelectionDialog.h"
#include "ui_ExportSelectionDialog.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

ExportSelectionDialog::ExportSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportSelectionDialog)
{
    ui->setupUi(this);

    ui->formatCombo->addItem("CSV");
}

ExportSelectionDialog::~ExportSelectionDialog()
{
    delete ui;
}

void ExportSelectionDialog::init(QTableView *tableView)
{
    m_tableView = tableView;
}

void ExportSelectionDialog::on_buttonBox_accepted()
{
    // Show dialog and get filename
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), tr("CSV Files (*.csv)"));

    // If the user chose a file to save to
    if (fileName.length()) {

            // Create file handle
            QFile file(fileName);

            // If we could successfully open the file
            if (file.open(QFile::WriteOnly | QFile::Truncate))
            {
                QTextStream data(&file);
                QStringList strList;

                // Cycle through each row
                for( int row = 0; row < m_tableView->model()->rowCount(); row++ )
                {
                    strList.clear();

                    // Cycle through each column
                    for( int column = 0; column < m_tableView->model()->columnCount(); column++ )
                    {
                        QModelIndex index = m_tableView->model()->index(row, column, QModelIndex());
                        strList << "\" " + m_tableView->model()->data(index).toString() + "\" ";
                    }
                    data << strList.join( "," )+"\n";
                }

                // Close file handle
                file.close();
            }
    }
}
