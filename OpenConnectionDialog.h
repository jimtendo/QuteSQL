#ifndef OPENCONNECTIONDIALOG_H
#define OPENCONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class OpenConnectionDialog;
}

struct SavedConnection
{
    QString name;
    QString driver;
    QString hostname;
    QString database;
    QString username;
    QString password;
    int port;
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
    bool    getSaveConnection();

private slots:
    void on_driverCombo_currentIndexChanged(const QString &arg1);

    void on_fileButton_clicked();

    void on_okButton_clicked();

    void on_connectionsComboBox_currentIndexChanged(int index);

private:
    Ui::OpenConnectionDialog *ui;

    QList<SavedConnection*> savedConnections;
};

#endif // OPENCONNECTIONDIALOG_H
