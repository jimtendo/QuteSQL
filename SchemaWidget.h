#ifndef SCHEMAWIDGET_H
#define SCHEMAWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>

#include <Extension/Extension.h>

class Extension;

class SchemaModel : public QSqlQueryModel
{
public:
    SchemaModel(QObject *parent, QSqlDatabase *database);

    void setTable(QString table);

    virtual QMap<QString, int> getDataTypes();



private:
    QSqlDatabase *m_database;
    QString m_tableName;
};

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

public slots:
    void refresh();

    void addColumn();
    void removeColumn();

signals:
    void refreshNeeded();

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
private slots:
    void on_tableView_activated(const QModelIndex &index);
};

#endif // SCHEMAWIDGET_H
