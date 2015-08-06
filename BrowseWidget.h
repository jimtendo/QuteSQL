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

private slots:
    void on_filterButton_clicked();

    void on_removeButton_clicked();

    void on_addButton_clicked();

    void on_clearButton_clicked();

    void currentChanged(QModelIndex previous, QModelIndex current);

    void commitChanges();

    void on_saveAsButton_clicked();

private:
    Ui::BrowseWidget *ui;

    // Database
    QSqlDatabase *m_database;

    // Model for table
    QSqlTableModel *m_model;

    // Current Table
    QString m_table;
};

#endif // BROWSEWIDGET_H
