#ifndef OPENCONNECTIONDIALOG_H
#define OPENCONNECTIONDIALOG_H

#include <QDialog>
#include <QListWidgetItem>


namespace Ui {
class OpenConnectionDialog;
}

struct SavedConnection
{
    // Database settings
    QString name;
    QString driver;
    QString hostname;
    QString database;
    QString username;
    QString password;
    int port;

    // SSH Tunnel Settings
    QString sshHostname;
    QString sshUsername;
    QString sshPassword;
    int sshPort;
};

class OpenConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenConnectionDialog(QWidget *parent = 0);
    ~OpenConnectionDialog();

    QString getName();
    QString getDriver();
    QString getDatabase();
    QString getHostname();
    QString getUsername();
    QString getPassword();
    int     getPort();

    // SSH Tunnel Functions
    bool    getSshTunnelChecked();
    QString getSshHostname();
    QString getSshUsername();
    QString getSshPassword();
    int     getSshPort();


private slots:
    void on_driverCombo_currentIndexChanged(const QString &arg1);

    void on_fileButton_clicked();

    void on_addButton_clicked();

    void on_connectionsListWidget_itemActivated(QListWidgetItem *item);

    void on_removeButton_clicked();

    void on_newConnectionButton_clicked();

    void on_sshTunnelCheckBox_toggled(bool checked);

private:
    void reloadConnections();

private:
    Ui::OpenConnectionDialog *ui;

    QList<SavedConnection*> savedConnections;
};

#endif // OPENCONNECTIONDIALOG_H
