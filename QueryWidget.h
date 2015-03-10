#ifndef QUERYWIDGET_H
#define QUERYWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>

namespace Ui {
class QueryWidget;
}

class QueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QueryWidget(QWidget *parent = 0);
    ~QueryWidget();

    void setDatabase(QSqlDatabase *database);

private slots:
    void on_runButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::QueryWidget *ui;

    // Database
    QSqlDatabase *m_database;

    // Model for query
    QSqlQueryModel *m_model;
};

#endif // QUERYWIDGET_H
