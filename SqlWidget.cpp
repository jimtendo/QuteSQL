#include "SqlWidget.h"
#include "ui_SqlWidget.h"

#include <QDebug>

#include <QFile>
#include <QFileDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

SqlWidget::SqlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SqlWidget)
{
    ui->setupUi(this);
}

SqlWidget::~SqlWidget()
{
    delete ui;
}

void SqlWidget::setDatabase(QSqlDatabase database)
{
    // Set the database
    m_database = database;
}

void SqlWidget::on_loadFromFileButton_clicked()
{
    // Show dialog and get filename
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/jana", tr("SQL Files (*.sql)"));

    // If no filename was specified, just return
    if (fileName.isEmpty()) {
        return;
    }

    // Otherwise, let's load that file
    QFile file(fileName);

    // If the file exists, open it
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        ui->sqlEdit->setPlainText(file.readAll());
    }
}

void SqlWidget::on_runSqlButton_clicked()
{
    // Get the query string
    QString queryString = ui->sqlEdit->toPlainText();

    // Split the query string into multiple queries
    QStringList queries = queryString.split(QRegularExpression(";"), QString::SkipEmptyParts);

    // Enable progress bar
    ui->progressBar->setEnabled(true);

    // Set progress bar range
    ui->progressBar->setRange(0, queries.length());

    foreach(QString statement, queries)
    {
        // Create query
        QSqlQuery query(m_database);

        // Execute the query string
        if (!query.exec(statement)) {

            // Always show debug message
            qDebug() << "Error: " << statement;

            // Only show error if the box is ticked
            if (ui->showErrorsCheckBox->isChecked()) {
                QMessageBox::critical(this, "Could not execute query", query.lastError().text());
            }
        }

        // Increment progress bar value
        ui->progressBar->setValue( ui->progressBar->value() + 1 );
    }

    QMessageBox::information(this, "SQL Executed Successfully", "SQL Code has been executed successfully.");
}
