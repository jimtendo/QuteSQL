#ifndef SCHEMAWIDGET_H
#define SCHEMAWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>

namespace Ui {
class SchemaWidget;
}

class SchemaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SchemaWidget(QWidget *parent = 0);
    ~SchemaWidget();

    void setDatabase(QSqlDatabase *database);

    bool setTable(QString table);

private:
    Ui::SchemaWidget *ui;

    // Database
    QSqlDatabase *m_database;

    // Model for query
    QSqlQueryModel *m_model;
};

#endif // SCHEMAWIDGET_H
