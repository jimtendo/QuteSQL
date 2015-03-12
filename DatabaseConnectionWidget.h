#ifndef DATABASECONNECTIONWIDGET_H
#define DATABASECONNECTIONWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>

#include "Extension/Extension.h"

namespace Ui {
class DatabaseConnectionWidget;
}

class DatabaseConnectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseConnectionWidget(QWidget *parent = 0);
    ~DatabaseConnectionWidget();

    QSqlDatabase database();

    QString name();

    bool connectToDatabase(QString name, QString driver, QString host, QString database, QString username, QString password, int port = 0);

    QSqlError lastError();

    Extension *getExtension();

public slots:
    void refresh();

private:
    Ui::DatabaseConnectionWidget *ui;

    // Database connection
    QSqlDatabase m_database;

    // Driver Extensions (Support for Tools, custom commands, etc)
    Extension *m_extension;
};

#endif // DATABASECONNECTIONWIDGET_H
