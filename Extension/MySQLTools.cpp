#include "MySQLTools.h"
#include "ui_MySQLTools.h"

#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

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
    if (!mysqldump.waitForFinished(240000)) {

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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("SQL Files (*.sql)"));

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
    if (!mysql.waitForFinished(240000)) {

        // Show message box if dump failed
        QMessageBox::critical(this, "Database Restore Failed", "Please ensure that you have mysql-client installed on this system.");

        // Prevent further execution
        return;
    }

    // Let user know backup was successful
    QMessageBox::information(this, "Restore Successful", "Database has been successfully restored.");
}

void MySQLTools::on_clearDatabaseButton_clicked()
{
    if (QMessageBox::Yes == QMessageBox::question(this, "Clear Database", "Are you sure you want to clear the database?\n\nThis action cannot be undone.")) {

        QSqlQuery query(*m_database);

        QString statements = "SET FOREIGN_KEY_CHECKS = 0; \
        SET GROUP_CONCAT_MAX_LEN=32768; \
        SET @tables = NULL; \
        SELECT GROUP_CONCAT('`', table_name, '`') INTO @tables \
          FROM information_schema.tables \
          WHERE table_schema = (SELECT DATABASE()); \
        SELECT IFNULL(@tables,'dummy') INTO @tables; \
        \
        SET @tables = CONCAT('DROP TABLE IF EXISTS ', @tables); \
        PREPARE stmt FROM @tables; \
        EXECUTE stmt; \
        DEALLOCATE PREPARE stmt; \
        SET FOREIGN_KEY_CHECKS = 1;";

        if (!query.exec(statements)) {

            // Only show error if the box is ticked
            QMessageBox::critical(this, "Could not execute query", query.lastError().text());

            // Prevent further execution
            return;
        }

        // Let the user know it all went splendidly
        QMessageBox::information(this, "Clear Successful", "Database has been successfully cleared.");

        // Let the app know a refresh is needed
        emit refreshNeeded();

    }
}
