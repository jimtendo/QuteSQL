#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QSettings>
#include <QToolBar>
#include <QTime>

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

        // Create a new database connection widget
        DatabaseConnectionWidget *databaseConnectionWidget = new DatabaseConnectionWidget(this);

        // Seed random number generator
        qsrand(QTime::currentTime().msec());

        // Get SSH Tunnel information from dialog
        QString sshHostname = connectionDialog.getSshHostname();
        int sshPort = connectionDialog.getSshPort();
        int sshForwardedPort = qrand() % 1000 + 17000; // Get a random port to forward to between 17000 and 18000

        // If we want this to be SSH tunneled
        if (connectionDialog.getSshTunnelChecked()) {

            // Try to create the tunnel
            if (!databaseConnectionWidget->createSshTunnel(sshHostname, connectionDialog.getPort(), sshForwardedPort)) {

                // Show message box if we couldn't connect
                QMessageBox::critical(this, "Error", "Could not create SSH Tunnel");

                // Update status
                emit statusEvent("Could not create SSH tunnel");

                // Delete this widget
                delete databaseConnectionWidget;

                // And return to prevent further execution
                return;
            }
        }

        // Get database information from dialog
        QString name = connectionDialog.getName();
        QString driver = connectionDialog.getDriver();
        QString hostname = connectionDialog.getHostname();
        QString database = connectionDialog.getDatabase();
        QString username = connectionDialog.getUsername();
        QString password = connectionDialog.getPassword();
        int port = connectionDialog.getSshTunnelChecked() ? sshForwardedPort : connectionDialog.getPort();

        // Connect to database
        if (!databaseConnectionWidget->connectToDatabase(name, driver, hostname, database, username, password, port)) {

            // Show message box if we couldn't connect
            QMessageBox::critical(this, "Could not connect", databaseConnectionWidget->lastError().databaseText());

            // Update status
            emit statusEvent(databaseConnectionWidget->lastError().databaseText());

            // Delete this widget
            delete databaseConnectionWidget;

            // And return to prevent further execution
            return;
        }

        // Create icon and this to the list of tabs
        ui->databaseConnectionsTabWidget->addTab(databaseConnectionWidget, QIcon::fromTheme("server-database"), databaseConnectionWidget->name());
        ui->databaseConnectionsTabWidget->setCurrentWidget(databaseConnectionWidget);

        // Set the current database
        m_currentDatabase = databaseConnectionWidget;

        // Emit status event
        emit statusEvent("Connection to database successful");
    }
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
    QMessageBox::about(this, "About QuteSQL", "QuteSQL developed by James Augustus Zuccon PhD");
}

void MainWindow::on_databaseConnectionsTabWidget_tabCloseRequested(int index)
{
    // Get a pointer to the tab instance so we can delete it later (at the end of this function)
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
        for (QList<QToolBar*>::iterator i = toolbars.begin(); i != toolbars.end(); ++i) {
            removeToolBar((*i));
        }
    }

    // Set the new active database
    m_currentDatabase = (DatabaseConnectionWidget*)ui->databaseConnectionsTabWidget->currentWidget();

    // Add new database's toolbars
    if (m_currentDatabase && m_currentDatabase->getExtension()) {

        QList<QToolBar*> toolbars = m_currentDatabase->getExtension()->getToolBars();
        for (QList<QToolBar*>::iterator i = toolbars.begin(); i != toolbars.end(); ++i) {
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

void MainWindow::on_actionRefresh_triggered()
{
    // If there is a current database, then refresh it
    if (m_currentDatabase) {
        m_currentDatabase->refresh();
    }
}
