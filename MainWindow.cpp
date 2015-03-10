#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QSettings>
#include <QToolBar>

#include "OpenConnectionDialog.h"
#include "DatabaseConnectionWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(statusEvent(QString)), ui->statusBar, SLOT(showMessage(QString)));

    m_currentDatabase = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Connection_triggered()
{
    // Create the open connection dialog
    OpenConnectionDialog connectionDialog(this);

    // Show the dialog and wait for user input
    if (connectionDialog.exec() == QDialog::Accepted) {

        // Get information from widget
        QString name = connectionDialog.getName();
        QString driver = connectionDialog.getDriver();
        QString hostname = connectionDialog.getHostname();
        QString database = connectionDialog.getDatabase();
        QString username = connectionDialog.getUsername();
        QString password = connectionDialog.getPassword();
        int port = connectionDialog.getPort();

        // Create a new database connection widget
        DatabaseConnectionWidget *databaseConnectionWidget = new DatabaseConnectionWidget(this);

        // Connect to database
        if (!databaseConnectionWidget->connectToDatabase(name, driver, hostname, database, username, password, port)) {

            // Show message box if we couldn't connect
            QMessageBox::critical(this, "Could not connect", databaseConnectionWidget->lastError().databaseText());

            // Update status
            emit statusEvent(databaseConnectionWidget->lastError().databaseText());

            // And return to prevent further execution
            return;
        }

        // Create icon and this to the list of tabs
        QIcon tabIcon = QIcon::fromTheme("database");
        ui->databaseConnectionsTabWidget->addTab(databaseConnectionWidget, tabIcon, databaseConnectionWidget->name());

        // Emit status event
        emit statusEvent("Connection to database successful");
    }
}

bool MainWindow::connectToDatabase(QString driver, QString host, QString database, QString username, QString password, int port)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(driver, host+database);
    db.setHostName(host);
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPassword(password);
    db.setPort(port);
    return db.open();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionAbout_QuteSql_triggered()
{
    QMessageBox::about(this, "About QuteSQL", "QuteSQL developed by James Augustus Zuccon");
}

void MainWindow::on_databaseConnectionsTabWidget_tabCloseRequested(int index)
{
    // Get a pointer to the tab instance so we can delete it later
    DatabaseConnectionWidget *widget = (DatabaseConnectionWidget*)ui->databaseConnectionsTabWidget->widget(index);

    // Remove the tab
    ui->databaseConnectionsTabWidget->removeTab(index);

    // Actually delete the widget
    delete widget;
}

void MainWindow::on_databaseConnectionsTabWidget_currentChanged(int index)
{
    // Remove old database's toolbars if there is one
    if (m_currentDatabase && m_currentDatabase->getExtension()) {

        QList<QToolBar*> toolbars = m_currentDatabase->getExtension()->getToolBars();
        QList<QToolBar*>::iterator i;
        for (i = toolbars.begin(); i != toolbars.end(); ++i) {
            (*i)->setHidden(true);
            removeToolBar((*i));
        }
    }

    // Set the new active database
    m_currentDatabase = (DatabaseConnectionWidget*)ui->databaseConnectionsTabWidget->currentWidget();

    // Add new database's toolbars
    if (m_currentDatabase && m_currentDatabase->getExtension()) {

        QList<QToolBar*> toolbars = m_currentDatabase->getExtension()->getToolBars();
        QList<QToolBar*>::iterator i;
        for (i = toolbars.begin(); i != toolbars.end(); ++i) {
            addToolBar((*i));
        }
    }
}

void MainWindow::on_actionClose_Connection_triggered()
{
    // Get the current tab index
    int currentTabIndex = ui->databaseConnectionsTabWidget->currentIndex();

    // Get a pointer to the widget that the current index refers to so we can delete it later
    DatabaseConnectionWidget *widget = (DatabaseConnectionWidget*)ui->databaseConnectionsTabWidget->widget(currentTabIndex);

    // Remove the tab
    ui->databaseConnectionsTabWidget->removeTab(currentTabIndex);

    // Delete it
    delete widget;
}
