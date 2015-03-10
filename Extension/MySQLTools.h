#ifndef MYSQLTOOLS_H
#define MYSQLTOOLS_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class MySQLTools;
}

class MySQLTools : public QWidget
{
    Q_OBJECT

public:
    explicit MySQLTools(QWidget *parent = 0);
    ~MySQLTools();

    void setDatabase(QSqlDatabase *database);

private slots:
    void on_backupDatabaseButton_clicked();

    void on_restoreDatabaseButton_clicked();

private:
    Ui::MySQLTools *ui;

    QSqlDatabase *m_database;
};

#endif // MYSQLTOOLS_H
