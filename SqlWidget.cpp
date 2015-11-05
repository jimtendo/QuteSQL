#include "SqlWidget.h"
#include "ui_SqlWidget.h"

#include <QDebug>

#include <QFile>
#include <QFileDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

#include "Utilities/SQLSplitter.h"

SqlWidget::SqlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SqlWidget),
    m_highlighter(NULL)
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("SQL Files (*.sql)"));

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
    SQLSplitter splitter(&queryString);

    // Enable progress bar
    ui->progressBar->setEnabled(true);

    // Set progress bar range
    ui->progressBar->setRange(0, splitter.getLength());

    while (!splitter.atEnd()) {
        // Create query
        QSqlQuery query(m_database);

        // Get next query
        QString statement = splitter.getNext();

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
        ui->progressBar->setValue(splitter.getPosition());
    }

    // Let the user know the code has been run
    QMessageBox::information(this, "SQL Executed Successfully", "SQL has been executed successfully.");

    // Emit a signal that the database needs refreshing
    emit refreshNeeded();
}

void SqlWidget::on_syntaxCheckBox_toggled(bool checked)
{
    // Attach SQL Highlighter if checked, otherwise delete it
    if (checked) {
        m_highlighter = new SQLHighlighter(ui->sqlEdit->document());
    } else {
        delete m_highlighter;
    }
}
