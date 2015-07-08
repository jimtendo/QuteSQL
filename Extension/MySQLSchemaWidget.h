#ifndef MYSQLSCHEMAWIDGET_H
#define MYSQLSCHEMAWIDGET_H

#include "SchemaWidget.h"
#include "Extension.h"

#include <QSqlDatabase>

class MySQLSchemaModel : public QSqlQueryModel
{
public:
    MySQLSchemaModel(QObject *parent, QSqlDatabase *database, Extension *extension = NULL);

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void setTable(QString table);

private:
    QSqlDatabase *m_database;
    QString m_tableName;
};

class MySQLSchemaWidget : public SchemaWidget
{
public:
    explicit MySQLSchemaWidget(QWidget *parent, QSqlDatabase *database);

    void init();

    bool setTable(QString table);
};

#endif // MYSQLSCHEMAWIDGET_H
