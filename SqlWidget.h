#ifndef SQLWIDGET_H
#define SQLWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

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
};

#endif // SQLWIDGET_H
