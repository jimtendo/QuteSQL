#ifndef SQLWIDGET_H
#define SQLWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

#include "SQLHighlighter.h"

namespace Ui {
class SqlWidget;
}

class SqlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SqlWidget(QWidget *parent = 0);
    ~SqlWidget();

    void setDatabase(QSqlDatabase database);

private slots:
    void on_loadFromFileButton_clicked();

    void on_runSqlButton_clicked();

signals:
    void refreshNeeded();

private:
    Ui::SqlWidget *ui;

    QSqlDatabase m_database;

    // Highlighter for SQL Code
    SQLHighlighter *m_highlighter;
};

#endif // SQLWIDGET_H
