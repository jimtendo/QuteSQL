#ifndef QUERYWIDGET_H
#define QUERYWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>

#include "SQLHighlighter.h"

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

    void on_exportButton_clicked();

    void saveQuery();

    void setQuery(QString query);

signals:
    void refreshNeeded();

private:
    Ui::QueryWidget *ui;

    // Database
    QSqlDatabase *m_database;

    // Model for query
    QSqlQueryModel *m_model;

    // Highlighter for SQL Code
    SQLHighlighter *m_highlighter;
};

#endif // QUERYWIDGET_H
