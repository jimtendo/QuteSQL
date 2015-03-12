#include "MySQLExtension.h"
#include "MySQLTools.h"

#include <QApplication>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QProcess>
#include <QMessageBox>

MySQLExtension::MySQLExtension(QObject *parent, QSqlDatabase *database) :
    Extension(parent, database)
{
    // Backup actions
    m_backupAction = new QAction(QIcon::fromTheme("filesave"), QT_TR_NOOP("&Backup..."), &m_toolsToolBar);
    m_backupAction->setStatusTip(QT_TR_NOOP("Backup database to file"));
    connect(m_backupAction, SIGNAL(triggered()), &m_toolsWidget, SLOT(on_backupDatabaseButton_clicked()));

    // Restore actions
    m_restoreAction = new QAction(QIcon::fromTheme("fileopen"), QT_TR_NOOP("&Restore..."), &m_toolsToolBar);
    m_restoreAction->setStatusTip(QT_TR_NOOP("Restore database to file"));
    connect(m_restoreAction, SIGNAL(triggered()), &m_toolsWidget, SLOT(on_restoreDatabaseButton_clicked()));

    // Clear actions
    m_clearAction = new QAction(QIcon::fromTheme("edit-clear"), QT_TR_NOOP("&Clear..."), &m_toolsToolBar);
    m_clearAction->setStatusTip(QT_TR_NOOP("Clear Database"));
    connect(m_clearAction, SIGNAL(triggered()), &m_toolsWidget, SLOT(on_clearDatabaseButton_clicked()));

    // Add actions to "Tools" toolbar
    m_toolsToolBar.addAction(m_backupAction);
    m_toolsToolBar.addAction(m_restoreAction);
    m_toolsToolBar.addAction(m_clearAction);

    // Append "Tools" toolbar
    m_toolBars.append(&m_toolsToolBar);

    // Create Tools Tab
    m_toolsWidget.setDatabase(m_database);
    m_toolsTab = new ExtensionTab(&m_toolsWidget, QIcon::fromTheme("applications-utilities"), "Tools");
    m_tabs.append(m_toolsTab);

    // Connect our signals
    connect(&m_toolsWidget, SIGNAL(refreshNeeded()), this, SIGNAL(refreshNeeded()));
}

MySQLExtension::~MySQLExtension()
{
    delete m_toolsTab;
    delete m_clearAction;
    delete m_backupAction;
    delete m_restoreAction;
}
