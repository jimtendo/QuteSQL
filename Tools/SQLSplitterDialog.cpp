#include "SQLSplitterDialog.h"
#include "ui_SQLSplitterDialog.h"

#include <QFileDialog>
#include <QString>
#include <QMessageBox>

#include "Utilities/SQLSplitter.h"

SQLSplitterDialog::SQLSplitterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SQLSplitterDialog)
{
    ui->setupUi(this);
}

SQLSplitterDialog::~SQLSplitterDialog()
{
    delete ui;
}

void SQLSplitterDialog::on_inputFileButton_clicked()
{
    // Show dialog and get filename
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("SQL Files (*.sql)"));

    // Set the input box filename
    ui->inputFileEdit->setText(fileName);
}

void SQLSplitterDialog::on_outputFileButton_clicked()
{
    // Show dialog and get filename
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"), QDir::homePath(), tr("SQL Files (*.sql)"));

    // Set the input box filename
    ui->outputFileEdit->setText(fileName);
}

void SQLSplitterDialog::on_buttonBox_accepted()
{
    // Setup files and variables
    int part = 1;
    int maxSize = ui->sizeSpinBox->text().toInt() * 1000;
    QFile inputFile(ui->inputFileEdit->text());
    QFile outputFile(ui->outputFileEdit->text() + "." + QString::number(part));

    // If the file exists, open it
    if (inputFile.open(QFile::ReadOnly | QFile::Text) && outputFile.open(QFile::WriteOnly | QFile::Text)) {
        SQLSplitter splitter(&inputFile);

        // Split the statements until the end
        while (!splitter.atEnd()) {
            QString nextStatement = splitter.getNext();

            // If the next statement is bigger than the max permitted size, abort!
            if (nextStatement.length() > maxSize) {
                QMessageBox::critical(this, "Unable to split file", "A statement in this file is larger than the permitted max file size.");
                inputFile.close();
                outputFile.close();
                return;
            }

            // Check if we need to start a new file
            if (outputFile.size() + nextStatement.length() > maxSize) {
                part++;
                outputFile.close();
                outputFile.setFileName(ui->outputFileEdit->text() + "." + QString::number(part));
                outputFile.open(QFile::WriteOnly | QFile::Text);
            }

            // Write the statement to the file
            outputFile.write(nextStatement.toUtf8());
        }

        // Close our files
        inputFile.close();
        outputFile.close();

        QMessageBox::information(this, "Operation Successful", "File has been successfully split.");
    }
}
