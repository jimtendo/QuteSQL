#include "MySQLExtension.h"
#include "MySQLTools.h"
#include "MySQLSchemaWidget.h"

#include <QApplication>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MySQLExtension::MySQLExtension(QObject *parent, QSqlDatabase *database) :
    Extension(parent, database)
{
    // Backup actions
    m_backupAction = new QAction(QIcon::fromTheme("document-export"), QT_TR_NOOP("&Export..."), &m_toolsToolBar);
    m_backupAction->setStatusTip(QT_TR_NOOP("Export database"));
    connect(m_backupAction, SIGNAL(triggered()), &m_toolsWidget, SLOT(on_backupDatabaseButton_clicked()));

    // Restore actions
    m_restoreAction = new QAction(QIcon::fromTheme("document-import"), QT_TR_NOOP("&Import..."), &m_toolsToolBar);
    m_restoreAction->setStatusTip(QT_TR_NOOP("Import Database"));
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

int MySQLExtension::hasCapability(Capability capability)
{
    switch (capability) {
        case ADD_TABLE: return true;
        case REMOVE_TABLE: return true;
        case RENAME_TABLE: return true;

        case VIEW_SCHEMA: return true;
        case ADD_COLUMN: return true;
        case REMOVE_COLUMN: return true;
    }

    return false;
}

int MySQLExtension::importDatabase()
{
    /*// Show dialog and get filename
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
    QMessageBox::information(this, "Restore Successful", "Database has been successfully restored.");*/
}

int MySQLExtension::exportDatabase()
{
    /*// Show dialog and get filename
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

    QMessageBox::information(this, "Backup Successful", "Database has been successfully backed up to:\n " + fileName);*/
}

int MySQLExtension::clearDatabase()
{
    /*if (QMessageBox::Yes == QMessageBox::question(this, "Clear Database", "Are you sure you want to clear the database?\n\nThis action cannot be undone.")) {

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
    }*/
}

int MySQLExtension::createTable(QString table)
{
    // Run the drop query
    QSqlQuery query = m_database->exec("CREATE TABLE " + table + "(id INT NOT NULL AUTO_INCREMENT, PRIMARY KEY(id))");

    return true;
}

int MySQLExtension::removeTable(QString table)
{
    // Run the drop query
    QSqlQuery query = m_database->exec("DROP TABLE " + table);

    return true;
}

int MySQLExtension::renameTable(QString from, QString to)
{
    // Run the rename query
    QSqlQuery query = m_database->exec("RENAME TABLE " + from + " TO " + to);

    return true;
}

QString MySQLExtension::viewSchemaQuery(QString table)
{
    return "DESC " + table;
}

int MySQLExtension::getSchemaColumn(SchemaColumn column)
{
    switch (column) {
        case NAME: return 0;
        case TYPE: return 1;
        case NULLABLE: return 2;
        case DEFAULT_VALUE: return 4;
    }

    return 0;
}

QMap<QString, int> MySQLExtension::getDataTypes()
{
    QMap<QString, int> types;

    types["char"] = HAS_LENGTH | HAS_NULLABLE | HAS_DEFAULT;
    types["varchar"] = HAS_LENGTH | HAS_NULLABLE | HAS_DEFAULT;
    types["tinytext"] = HAS_LENGTH | HAS_NULLABLE | HAS_DEFAULT;
    types["text"] = HAS_LENGTH | HAS_NULLABLE | HAS_DEFAULT;
    types["mediumtext"] = HAS_LENGTH | HAS_NULLABLE | HAS_DEFAULT;
    types["longtext"] = HAS_LENGTH | HAS_NULLABLE | HAS_DEFAULT;
    types["binary"] = HAS_LENGTH | HAS_NULLABLE | HAS_DEFAULT;
    types["varbinary"] = HAS_LENGTH | HAS_NULLABLE | HAS_DEFAULT;

    types["text"] = NO_PROPERTIES;
    types["varchar"] = HAS_LENGTH | HAS_NULLABLE | HAS_DEFAULT;
    types["int"] = HAS_LENGTH + HAS_NULLABLE + HAS_DEFAULT;
    types["date"] = NO_PROPERTIES;;
    types["datetime"] = NO_PROPERTIES;;
    types["timestamp"] = NO_PROPERTIES;;

    return types;
}

int MySQLExtension::addColumn(QString table, QString name, QString type, int length, bool nullable, QString defaultValue)
{
    QString queryString = "ALTER TABLE " + table + " ADD " + name;

    // If there's a length, embed that in type
    if (length) {
        type = type + "(" + QString::number(length) + ")";
    }

    // Add type
    queryString += " " + type;

    // If nullable is set, add that
    if (!nullable) {
        queryString += " NOT NULL";
    }

    // if default value is set, add that
    if (defaultValue.length()) {
        queryString += " DEFAULT '" + defaultValue + "'";
    }

    // Run the add column query
    QSqlQuery query = m_database->exec(queryString);

    return true;
}

int MySQLExtension::removeColumn(QString table, QString column)
{
    // Run the rename query
    QSqlQuery query = m_database->exec("ALTER TABLE " + table + " DROP COLUMN " + column);

    return true;
}

SchemaWidget* MySQLExtension::createSchemaWidget(QWidget *parent)
{
    return new MySQLSchemaWidget(parent, m_database);
}
