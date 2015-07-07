#include "OpenConnectionDialog.h"
#include "ui_OpenConnectionDialog.h"

#include <QSqlDatabase>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>

OpenConnectionDialog::OpenConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenConnectionDialog)
{
    ui->setupUi(this);

    // Load saved connections
    reloadConnections();

    // Setup list of drivers for selection
    foreach (QString driver, QSqlDatabase::drivers()) {
        ui->driverCombo->addItem(driver);
    }
}

OpenConnectionDialog::~OpenConnectionDialog()
{
    // Clear out SavedConnections
    qDeleteAll(savedConnections);
    savedConnections.clear();

    // Delete UI
    delete ui;
}

Connection * OpenConnectionDialog::getConnection()
{
    Connection *connection = new Connection;
    connection->driver = ui->driverCombo->currentText();
}

QString OpenConnectionDialog::getName()
{
    if (getDriver() == "QMYSQL" || getDriver() == "QMYSQL3") {
        QString name = getDatabase() + " - " + getUsername() + "@" + getHostname() + ":" + QString::number(getPort());

        if (getSshTunnelChecked()) {
            name += " (" + getSshHostname() + ":" + QString::number(getSshPort()) + ")";
        }

        return name;
    }

    QUrl url(ui->databaseEdit->text());

    return url.fileName();
}

QString OpenConnectionDialog::getDriver()
{
    return ui->driverCombo->currentText();
}

QString OpenConnectionDialog::getHostname()
{
    return ui->hostnameEdit->text();
}

QString OpenConnectionDialog::getDatabase()
{
    return ui->databaseEdit->text();
}

QString OpenConnectionDialog::getUsername()
{
    return ui->usernameEdit->text();
}

QString OpenConnectionDialog::getPassword()
{
    return ui->passwordEdit->text();
}

int OpenConnectionDialog::getPort()
{
    return ui->portSpinBox->text().toInt();
}

bool OpenConnectionDialog::getSshTunnelChecked()
{
    return ui->sshTunnelCheckBox->isChecked();
}

QString OpenConnectionDialog::getSshHostname()
{
    return ui->sshHostnameEdit->text();
}

int OpenConnectionDialog::getSshPort()
{
    return ui->sshPortSpinBox->text().toInt();
}

void OpenConnectionDialog::on_driverCombo_currentIndexChanged(const QString &arg1)
{
    // Disable fields if using SQLite
    if (arg1 == "QSQLITE") {
        ui->fileButton->setEnabled(true);
        ui->usernameEdit->setEnabled(false);
        ui->passwordEdit->setEnabled(false);
        ui->hostnameEdit->setEnabled(false);
        ui->portSpinBox->setEnabled(false);
        ui->sshTunnelCheckBox->setEnabled(false);
    }

    // Enable all fields if using MySQL
    else {
        ui->fileButton->setEnabled(false);
        ui->usernameEdit->setEnabled(true);
        ui->passwordEdit->setEnabled(true);
        ui->hostnameEdit->setEnabled(true);
        ui->portSpinBox->setEnabled(true);
        ui->portSpinBox->setValue(3306);
        ui->sshTunnelCheckBox->setEnabled(true);
    }
}

void OpenConnectionDialog::on_fileButton_clicked()
{
    // Show dialog and get filename
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("SQL Files (*.sqlite)"));

    ui->databaseEdit->setText(fileName);
}

void OpenConnectionDialog::on_addButton_clicked()
{
    // Only add connection if there's actually text there
    if (getName().length()) {
        // Add connection from settings to list
        Connection *newConnection = new Connection;
        newConnection->name = getName();
        newConnection->database = getDatabase();
        newConnection->driver = getDriver();
        newConnection->hostname = getHostname();
        newConnection->username = getUsername();
        newConnection->password = getPassword();
        newConnection->port = getPort();
        newConnection->sshTunnel = getSshTunnelChecked();
        newConnection->sshHostname = getSshHostname();
        newConnection->sshPort = getSshPort();

        // Append to saved connections
        savedConnections.append(newConnection);

        // Save connections
        QSettings settings;
        settings.beginWriteArray("connections");

        int i = 0;
        foreach(Connection* connection, savedConnections)
        {
            settings.setArrayIndex(i);

            // Add connection from settings to list
            settings.setValue("name", connection->name);
            settings.setValue("database", connection->database);
            settings.setValue("driver", connection->driver);
            settings.setValue("hostname", connection->hostname);
            settings.setValue("username", connection->username);
            settings.setValue("password", connection->password);
            settings.setValue("port", connection->port);
            settings.setValue("sshTunnel", connection->sshTunnel);
            settings.setValue("sshHostname", connection->sshHostname);
            settings.setValue("sshPort", connection->sshPort);
            i++;
        }
        settings.endArray();

        // Reload connections widget
        reloadConnections();
    }
}

void OpenConnectionDialog::on_connectionsListWidget_itemActivated(QListWidgetItem *item)
{
    Connection *connection;

    QList<Connection*>::iterator i;
    for (i = savedConnections.begin(); i != savedConnections.end(); ++i) {

        if ((*i)->name == item->text()) {
            connection = *i;
            break;
        }
    }

    int driver = ui->driverCombo->findText(connection->driver);
    ui->driverCombo->setCurrentIndex(driver);
    ui->databaseEdit->setText(connection->database);
    ui->usernameEdit->setText(connection->username);
    ui->passwordEdit->setText(connection->password);
    ui->hostnameEdit->setText(connection->hostname);
    ui->portSpinBox->setValue(connection->port);
    ui->sshTunnelCheckBox->setChecked(connection->sshTunnel);
    ui->sshHostnameEdit->setText(connection->sshHostname);
    ui->sshPortSpinBox->setValue(connection->sshPort);
}

void OpenConnectionDialog::reloadConnections()
{
    // Clear out SavedConnections
    qDeleteAll(savedConnections);
    savedConnections.clear();

    // Clear out the list widget
    ui->connectionsListWidget->clear();

    // Load saved connections
    QSettings settings;
    int size = settings.beginReadArray("connections");
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);

        // Add connection from settings to list
        Connection *connection = new Connection;
        connection->name = settings.value("name").toString();
        connection->database = settings.value("database").toString();
        connection->driver = settings.value("driver").toString();
        connection->hostname = settings.value("hostname").toString();
        connection->username = settings.value("username").toString();
        connection->password = settings.value("password").toString();
        connection->port = settings.value("port").toInt();
        connection->sshTunnel = settings.value("sshTunnel").toBool();
        connection->sshHostname = settings.value("sshHostname").toString();
        connection->sshPort = settings.value("sshPort").toInt();

        // Append to saved connections
        savedConnections.append(connection);

        // Add item to combo box
        ui->connectionsListWidget->addItem(connection->name);
    }
    settings.endArray();
}

void OpenConnectionDialog::on_removeButton_clicked()
{
    // Get current item
    QListWidgetItem *item = ui->connectionsListWidget->currentItem();

    if (item) {
        QList<Connection*>::iterator i;
        for (i = savedConnections.begin(); i != savedConnections.end(); ++i) {
            if (item->text() == "-- New Connection --") {
                break;
            }

            if ((*i)->name == item->text()) {
                savedConnections.removeOne(*i);
                break;
            }
        }

        // Save connections
        QSettings settings;
        settings.beginWriteArray("connections");

        int j = 0;
        foreach(Connection* connection, savedConnections)
        {
            settings.setArrayIndex(j);

            // Add connection from settings to list
            settings.setValue("name", connection->name);
            settings.setValue("database", connection->database);
            settings.setValue("driver", connection->driver);
            settings.setValue("hostname", connection->hostname);
            settings.setValue("username", connection->username);
            settings.setValue("password", connection->password);
            settings.setValue("port", connection->port);
            settings.setValue("sshTunnel", connection->sshTunnel);
            settings.setValue("sshHostname", connection->sshHostname);
            settings.setValue("sshPort", connection->sshPort);

            j++;
        }
        settings.endArray();

        // Reload connections widget
        reloadConnections();
    }
}

void OpenConnectionDialog::on_newConnectionButton_clicked()
{
    Connection *connection = new Connection;
    int driver = ui->driverCombo->findText(connection->driver);
    ui->driverCombo->setCurrentIndex(driver);
    ui->databaseEdit->setText(connection->database);
    ui->usernameEdit->setText(connection->username);
    ui->passwordEdit->setText(connection->password);
    ui->hostnameEdit->setText(connection->hostname);
    ui->portSpinBox->setValue(connection->port);
    ui->sshTunnelCheckBox->setChecked(false);
}

void OpenConnectionDialog::on_sshTunnelCheckBox_toggled(bool checked)
{
    ui->sshGroupBox->setEnabled(checked);
    ui->sshGroupBox->setVisible(checked);
    ui->hostnameEdit->setReadOnly(checked);

    if (checked) {
        ui->hostnameEdit->setText("127.0.0.1");
    }
}

void OpenConnectionDialog::initDatabase()
{
    QString connectionsQuery =
    "CREATE TABLE connections(id INT PRIMARY KEY NOT NULL,"
    "                         driver TEXT NOT NULL,"
    "                         name TEXT NOT NULL,"
    "                         database TEXT NOT NULL,"
    "                         username TEXT,"
    "                         password TEXT,"
    "                         hostname TEXT,"
    "                         port INTEGER,"
    "                         sshTunnel INTEGER,"
    "                         sshHostname TEXT,"
    "                         sshPort INTEGER"
    "                         );";

    QString savedQuery =
    "CREATE TABLE saved(id INTEGER PRIMARY KEY NOT NULL,"
    "                   name TEXT NOT NULL,"
    "                   command TEXT NOT NULL,"
    "                   connection_id INTEGER NOT NULL"
    "                   FOREIGN KEY(connection_id) REFERENCES connections(id)"
    "                   );";
}
