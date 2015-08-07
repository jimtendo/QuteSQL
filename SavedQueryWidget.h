#ifndef SAVEDQUERYWIDGET_H
#define SAVEDQUERYWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QListWidgetItem>

namespace Ui {
class SavedQueryWidget;
}

class SavedQueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SavedQueryWidget(QWidget *parent = 0);
    ~SavedQueryWidget();

    void setDatabase(QSqlDatabase *database);

    void addQuery(QString query);

private:
    Ui::SavedQueryWidget *ui;

    QSqlDatabase *m_database;

    void saveQueries();
    void loadQueries();

signals:
    void addButtonClicked();
    void querySelected(const QString &);

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_listWidget_itemActivated(QListWidgetItem *item);
};

#endif // SAVEDQUERYWIDGET_H
