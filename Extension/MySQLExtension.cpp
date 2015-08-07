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
#include <QProgressDialog>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>
#include <QDebug>

#define CREATE_TABLE_COLUMN 1

MySQLExtension::MySQLExtension(QObject *parent, QSqlDatabase *database) :
    Extension(parent, database)
{
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
}

int MySQLExtension::hasCapability(Capability capability)
{
    switch (capability) {
        case EXPORT_DATABASE: return true;
        case CLEAR_DATABASE: return true;

        case ADD_TABLE: return true;
        case REMOVE_TABLE: return true;
        case RENAME_TABLE: return true;

        case VIEW_SCHEMA: return true;
        case ADD_COLUMN: return true;
        case REMOVE_COLUMN: return true;
        case ALTER_COLUMN: return true;
    }

    return false;
}

int MySQLExtension::exportDatabase()
{
    // Show dialog and get filename

    QString fileName = QFileDialog::getSaveFileName(qApp->activeWindow(), tr("Save File"), QDir::homePath(), tr("SQL Files (*.sql)"));

    // If no filename was specified, just return
    if (fileName.isEmpty()) {
        return false;
    }

    // Setup file
    QFile outputFile(fileName);
    outputFile.open(QFile::WriteOnly | QFile::Text);

    // Get number of records we'll need to insert (for the progress dialog) as they are what takes times
    int insertsCount = 0;
    foreach (QString table, m_database->tables()) {
        QSqlQuery tableSizeQuery("SELECT COUNT(*) FROM " + table, *m_database);
        tableSizeQuery.next();
        insertsCount += tableSizeQuery.record().value(0).toInt();
    }

    // Create Progress Dialog
    QProgressDialog progress("Dumping database...", "Abort", 0, insertsCount);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);
    progress.show();

    // Create text stream and about headers
    QTextStream outputStream(&outputFile);
    outputStream << "-- QuteSQL Dump - V" << qApp->applicationVersion() << endl;
    outputStream << "SET NAMES utf8;" << endl;
    outputStream << "SET time_zone = '+00:00';" << endl;
    outputStream << "SET foreign_key_checks = 0;" << endl;
    outputStream << "SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';" << endl << endl;

    // Dump each table
    foreach (QString table, m_database->tables()) {

        // If the cancel button was clicked, abort
        if (progress.wasCanceled())
            return false;

        // Dump the create table statement
        QSqlQuery createTableQuery("SHOW CREATE TABLE " + table, *m_database);
        outputStream << "DROP TABLE IF EXISTS `" + table + "`;" << endl;
        createTableQuery.next();
        outputStream << createTableQuery.record().value(CREATE_TABLE_COLUMN).toString() + ";" << endl << endl;

        // Dump the insert queries
        QSqlQuery selectTableQuery("SELECT * FROM " + table, *m_database);
        if (selectTableQuery.size()) {
            while (selectTableQuery.next()) {
                QString row = "INSERT INTO `" + table + "` VALUES (";
                for (int column = 0; column < selectTableQuery.record().count(); column++) {
                    row += m_database->driver()->formatValue(selectTableQuery.record().field(column)) + ",";
                }
                row.truncate(row.size()-1);
                row += ");";
                outputStream << row << endl;

                // If the cancel button was clicked, abort
                progress.setValue(progress.value()+1);
                if (progress.wasCanceled())
                    return false;

                // Update our event loop so that progress dialog updates
                QCoreApplication::processEvents();
            }
        }
    }

    // Close our output file
    outputFile.close();

    QMessageBox::information(qApp->activeWindow(), "Backup Successful", "Database has been successfully exported to:\n " + fileName);

    return true;
}

int MySQLExtension::clearDatabase()
{
    if (QMessageBox::Yes == QMessageBox::question(qApp->activeWindow(), "Clear Database", "Are you sure you want to clear the database?\n\nThis action cannot be undone.")) {

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
            QMessageBox::critical(qApp->activeWindow(), "Could not execute query", query.lastError().text());

            // Prevent further execution
            return false;
        }

        // Let the user know it all went splendidly
        QMessageBox::information(qApp->activeWindow(), "Clear Successful", "Database has been successfully cleared.");

        // Let the app know a refresh is needed
        emit refreshNeeded();

        return true;
    }
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
    QSqlQuery query = m_database->exec("SET foreign_key_checks = 0; DROP TABLE " + table + "; SET foreign_key_checks = 1;");

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
    types["int unsigned"] = HAS_LENGTH + HAS_NULLABLE + HAS_DEFAULT;
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

int MySQLExtension::alterColumn(QString table, QString oldName, QString newName, QString type, int length, bool nullable, QString defaultValue)
{
    QString queryString = "ALTER TABLE " + table + " CHANGE " + oldName + " " + newName;

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

SchemaWidget* MySQLExtension::createSchemaWidget(QWidget *parent)
{
    return new MySQLSchemaWidget(parent, m_database);
}
