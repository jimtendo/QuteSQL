#include "MySQLTools.h"
#include "ui_MySQLTools.h"

#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDir>
#include <QDebug>

MySQLTools::MySQLTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MySQLTools)
{
    ui->setupUi(this);
}

MySQLTools::~MySQLTools()
{
    delete ui;
}

void MySQLTools::setDatabase(QSqlDatabase *database)
{
    m_database = database;
}

void MySQLTools::on_backupDatabaseButton_clicked()
{
    // Show dialog and get filename
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), tr("SQL Files (*.sql)"));

    // If no filename was specified, just return
    if (fileName.isEmpty()) {
        return;
    }

    // Create mysqldump proces and set file to save to
    QProcess mysqldump;
    mysqldump.setStandardOutputFile(fileName);

    // Compile Arguments
    QStringList arguments;
    arguments << "-h" << m_database->hostName() << "-P" << QString::number(m_database->port())
              << "-u" << m_database->userName() << QString("-p" + m_database->password()) << m_database->databaseName();

    // Execute dump command
    mysqldump.start("mysqldump", arguments);

    // Wait until finished
    if (!mysqldump.waitForFinished()) {

        // Show message box if dump failed
        QMessageBox::critical(this, "Database Backup Failed", "Please ensure that you have mysqldump installed on this system.");

        // Prevent further execution
        return;
    }

    QMessageBox::information(this, "Backup Successful", "Database has been successfully backed up to:\n " + fileName);
}

void MySQLTools::on_restoreDatabaseButton_clicked()
{
    // Show dialog and get filename
    QString fileName = QFileDialog::getOpenFileName(this, tr("Save File"), QDir::homePath(), tr("SQL Files (*.sql)"));

    // If no filename was specified, just return
    if (fileName.isEmpty()) {
        return;
    }

    // Create mysqldump proces and set file to save to
    QProcess mysql;
    mysql.setStandardInputFile(fileName);

    // Compile Arguments
    QStringList arguments;
    arguments << "-h" << m_database->hostName() << "-P" << QString::number(m_database->port())
              << "-u" << m_database->userName() << QString("-p" + m_database->password()) << m_database->databaseName();

    // Execute dump command
    mysql.start("mysql", arguments);

    // Wait until finished
    if (!mysql.waitForFinished()) {

        // Show message box if dump failed
        QMessageBox::critical(this, "Database Restore Failed", "Please ensure that you have mysql-client installed on this system.");

        // Prevent further execution
        return;
    }

    // Let user know backup was successful
    QMessageBox::information(this, "Restore Successful", "Database has been successfully restored");
}
