#ifndef SAVEDQUERYWIDGET_H
#define SAVEDQUERYWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

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

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
};

#endif // SAVEDQUERYWIDGET_H
