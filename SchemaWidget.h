#ifndef SCHEMAWIDGET_H
#define SCHEMAWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>

#include "Extension/Extension.h"

class Extension;

namespace Ui {
class SchemaWidget;
}

class SchemaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SchemaWidget(QWidget *parent, QSqlDatabase *database, Extension *extension = NULL);
    ~SchemaWidget();

    virtual void init();

    virtual bool setTable(QString table);

protected:
    Ui::SchemaWidget *ui;

    // Database
    QSqlDatabase *m_database;

    // Extension
    Extension *m_extension;

    // Current table Name
    QString m_tableName;

    // Model for query
    QSqlQueryModel *m_model;
};

#endif // SCHEMAWIDGET_H
