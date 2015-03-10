#include "DatabaseConnectionWidget.h"
#include "ui_DatabaseConnectionWidget.h"

#include <QUrl>
#include <QDebug>

#include "Extension/MySQLExtension.h"

DatabaseConnectionWidget::DatabaseConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseConnectionWidget)
{
    m_extension = NULL;

    ui->setupUi(this);
}

DatabaseConnectionWidget::~DatabaseConnectionWidget()
{
    m_database.close();

    delete m_extension;

    delete ui;
}

QSqlDatabase DatabaseConnectionWidget::database()
{
    return m_database;
}

QString DatabaseConnectionWidget::name()
{
    // If we're using SQLite, get the filename
    if (m_database.driverName() == "QSQLITE") {
        QUrl url(m_database.databaseName());
        return url.fileName();
    }

    if (m_database.driverName() == "QMYSQL") {
        return QString(m_database.databaseName() + " - " + m_database.userName() + "@" + m_database.hostName() + ":" + QString::number(m_database.port()));
    }

    return m_database.databaseName();
}

QSqlError DatabaseConnectionWidget::lastError()
{
    return m_database.lastError();
}

bool DatabaseConnectionWidget::connectToDatabase(QString driver, QString host, QString database, QString username, QString password, int port)
{
    m_database = QSqlDatabase::addDatabase(driver, QString(database + " - " + username + "@" + host + ":" + port));
    m_database.setHostName(host);
    m_database.setDatabaseName(database);
    m_database.setUserName(username);
    m_database.setPassword(password);
    m_database.setPort(port);

    qDebug() << m_database.hostName();

    // Attempt to connect
    if (!m_database.open()) {
        return false;
    }

    // Setup tabs
    ui->explorerTab->setDatabase(&m_database);
    ui->queryTab->setDatabase(m_database);
    ui->sqlTab->setDatabase(m_database);

    // If there's extra support for this driver, add it
    if (driver == "QMYSQL" || driver == "QMYSQL3") {

        // Create the extension (adds Tools widget, etc)
        m_extension = new MySQLExtension(this);
        m_extension->setDatabase(&m_database);
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

    return true;
}

Extension *DatabaseConnectionWidget::getExtension()
{
    return m_extension;
}
