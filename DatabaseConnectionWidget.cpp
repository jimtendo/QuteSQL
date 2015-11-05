#include "DatabaseConnectionWidget.h"
#include "ui_DatabaseConnectionWidget.h"

#include <QUrl>
#include <QThread>
#include <QDebug>
#include <QProgressDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>

#include "Extension/MySQLExtension.h"
#include "Utilities/QCompressor.h"
#include "Utilities/SQLSplitter.h"

DatabaseConnectionWidget::DatabaseConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseConnectionWidget)
{
    m_extension = NULL;

    ui->setupUi(this);
}

DatabaseConnectionWidget::~DatabaseConnectionWidget()
{
    // Close the ssh tunnel
    m_sshTunnel.terminate();

    // Close this connection
    m_database.close();

    // Remove the actual database (is this necessary if I close it?)
    QSqlDatabase::removeDatabase(m_database.connectionName());

    delete m_extension;

    delete ui;
}

QSqlDatabase DatabaseConnectionWidget::database()
{
    return m_database;
}

QString DatabaseConnectionWidget::name()
{
    return m_database.connectionName();
}

QSqlError DatabaseConnectionWidget::lastError()
{
    return m_database.lastError();
}

bool DatabaseConnectionWidget::createSshTunnel(QString hostname, int remotePort, int forwardedPort, int sshPort)
{
    // Compile Arguments (of the form: -fNg -L 16111:127.0.0.1:3306 user@www.hostname.com
    QStringList arguments;
    arguments << "-fNg" << "-L" << QString(QString::number(forwardedPort) + ":" + "127.0.0.1" + ":" + QString::number(remotePort)) << QString(hostname) << "-p" << QString::number(sshPort);

    // Execute SSH Command
    m_sshTunnel.start("ssh", arguments);

    // Wait until finished
    if (!m_sshTunnel.waitForFinished(30000)) {
        return false;
    }

    //  Print useful information
    qDebug() << QString("SSH tunnel binded to 127.0.0.1:" + QString::number(forwardedPort));

    return true;
}

bool DatabaseConnectionWidget::connectToDatabase(QString name, QString driver, QString host, QString database, QString username, QString password, int port)
{
    m_database = QSqlDatabase::addDatabase(driver, name);
    m_database.setHostName(host);
    m_database.setDatabaseName(database);
    m_database.setUserName(username);
    m_database.setPassword(password);
    m_database.setPort(port);

    // Attempt to connect
    if (!m_database.open()) {
        return false;
    }

    // If there's extra support for this driver, add it
    if (driver == "QMYSQL" || driver == "QMYSQL3") {

        // Create the extension (adds Tools widget, etc)
        m_extension = new MySQLExtension(this, &m_database);
        connect(m_extension, SIGNAL(refreshNeeded()), this, SLOT(refresh()));
    }

    // Setup the extensions if they exist
    if (m_extension) {

        // Add each tab provided by this extension
        QList<ExtensionTab*> tabs = m_extension->getTabs();
        QList<ExtensionTab*>::iterator i;
        for (i = tabs.begin(); i != tabs.end(); ++i) {
            ui->tabWidget->addTab((*i)->getWidget(), (*i)->getIcon(), (*i)->getLabel());
        }
    }

    // Setup Explorer Widget
    ui->explorerTab->init(&m_database, m_extension);

    // Setup Query Widget
    ui->queryTab->setDatabase(&m_database);
    connect(ui->queryTab, SIGNAL(refreshNeeded()), this, SLOT(refresh()));

    // Setup SQL Widget
    ui->sqlTab->setDatabase(m_database);
    connect(ui->sqlTab, SIGNAL(refreshNeeded()), this, SLOT(refresh()));

    return true;
}

bool DatabaseConnectionWidget::importDatabase()
{
    // Show dialog and get filename
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("SQL Files (*.sql *gz)"));

    // If no filename was specified, just return
    if (fileName.isEmpty()) {
        return false;
    }

    // Get the query string
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    // File data
    //QByteArray fileData = file.readAll();
    QByteArray decompressed;

    // Decompress if it is a gzip file
    QFileInfo fileInfo(fileName);
    if (fileInfo.suffix() == "gz") {
        /*if (!QCompressor::gzipDecompress(fileData, decompressed)) {
            return false;
        }*/

        //decompressed = QCompressor::gzDecompress(fileData);

        //qDebug() << decompressed;

        // Let the user know that this is broken
        QMessageBox::warning(this, "Operation Failed", "Compressed databases are not supported at this time.");

        return false;
    }

    // Read string from file and feed into splitter
    SQLSplitter splitter(&file);

    // Create Progress Dialog
    QProgressDialog progress("Importing database...", "Abort", 0, splitter.getLength());
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setValue(0);
    progress.show();

    while (!splitter.atEnd()) {
        // Create query
        QSqlQuery query(m_database);

        // Get next query
        QString statement = splitter.getNext();

        // Execute the query string
        if (!query.exec(statement)) {
            qDebug() << "Error: " << statement;
        }

        // If the cancel button was clicked, abort
        if (progress.wasCanceled())
            return false;

        // Increment progress bar value
        progress.setValue(splitter.getPosition());

        // Update our event loop so that progress dialog updates
        QCoreApplication::processEvents();
    }

    // Let the user know the code has been run
    QMessageBox::information(this, "Operation Successful", "Database has been imported successfully.");

    // Refresh the database
    refresh();

    return true;
}

Extension *DatabaseConnectionWidget::getExtension()
{
    return m_extension;
}

void DatabaseConnectionWidget::refresh()
{
    ui->explorerTab->refresh();
}
