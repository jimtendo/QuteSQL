#ifndef BROWSEWIDGET_H
#define BROWSEWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class BrowseWidget;
}

class BrowseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BrowseWidget(QWidget *parent = 0);
    ~BrowseWidget();

    void setDatabase(QSqlDatabase *database);

    bool setTable(QString table);

private:
    Ui::BrowseWidget *ui;

    // Database
    QSqlDatabase *m_database;

    // Model for table
    QSqlTableModel *m_model;
};

#endif // BROWSEWIDGET_H
