#include "MySQLExtension.h"
#include "MySQLTools.h"

#include <QApplication>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QProcess>
#include <QMessageBox>

MySQLExtension::MySQLExtension(QObject *parent) :
    Extension(parent)
{
    // Backup actions
    m_backupAction = new QAction(QIcon::fromTheme("filesave"), QT_TR_NOOP("&Backup..."), &m_toolsToolBar);
    m_backupAction->setStatusTip(QT_TR_NOOP("Backup database to file"));
    connect(m_backupAction, SIGNAL(triggered()), this, SLOT(backupDatabase()));

    // Restore actions
    m_restoreAction = new QAction(QIcon::fromTheme("fileopen"), QT_TR_NOOP("&Restore..."), &m_toolsToolBar);
    m_restoreAction->setStatusTip(QT_TR_NOOP("Restore database to file"));
    connect(m_restoreAction, SIGNAL(triggered()), this, SLOT(restoreDatabase()));

    // Add actions to "Tools" toolbar
    m_toolsToolBar.addAction(m_backupAction);
    m_toolsToolBar.addAction(m_restoreAction);

    // Append "Tools" toolbar
    m_toolBars.append(&m_toolsToolBar);

    // Create Tools Tab
    m_toolsWidget.setDatabase(m_database);
    ExtensionTab *toolsTab = new ExtensionTab(&m_toolsWidget, QIcon::fromTheme("applications-utilities"), "Tools");
    m_tabs.append(toolsTab);
}

MySQLExtension::~MySQLExtension()
{
    delete m_backupAction;
    delete m_restoreAction;
}

void MySQLExtension::backupDatabase()
{
    // Show dialog and get filename
    QString fileName = QFileDialog::getSaveFileName(QApplication::activeWindow(), tr("Save File"), QDir::homePath(), tr("SQL Files (*.sql)"));

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
        QMessageBox::critical(QApplication::activeWindow(), "Database Backup Failed", "Please ensure that you have mysqldump installed on this system.");

        // Prevent further execution
        return;
    }

    QMessageBox::information(QApplication::activeWindow(), "Backup Successful", "Database has been successfully backed up to:\n " + fileName);
}

void MySQLExtension::restoreDatabase()
{
    // Show dialog and get filename
    QString fileName = QFileDialog::getOpenFileName(QApplication::activeWindow(), tr("Save File"), QDir::homePath(), tr("SQL Files (*.sql)"));

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
        QMessageBox::critical(QApplication::activeWindow(), "Database Restore Failed", "Please ensure that you have mysql-client installed on this system.");

        // Prevent further execution
        return;
    }

    // Let user know backup was successful
    QMessageBox::information(QApplication::activeWindow(), "Restore Successful", "Database has been successfully restored");
}
