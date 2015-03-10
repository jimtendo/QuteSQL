#ifndef EXPLORERWIDGET_H
#define EXPLORERWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QListWidgetItem>

namespace Ui {
class ExplorerWidget;
}

class ExplorerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerWidget(QWidget *parent = 0);
    ~ExplorerWidget();

    void setDatabase(QSqlDatabase *database);

private slots:
    void on_tableListWidget_itemActivated(QListWidgetItem *item);

private:
    Ui::ExplorerWidget *ui;

    // Database
    QSqlDatabase *m_database;
};

#endif // EXPLORERWIDGET_H
