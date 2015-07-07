#ifndef QUERYHISTORYWIDGET_H
#define QUERYHISTORYWIDGET_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class QueryHistoryWidget;
}

class QueryHistoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QueryHistoryWidget(QWidget *parent = 0);
    ~QueryHistoryWidget();

    void addQuery(QString query);

signals:
    void querySelected(const QString&);

private slots:
    void on_commandListWidget_itemActivated(QListWidgetItem *item);

    void on_clearButton_clicked();

private:
    Ui::QueryHistoryWidget *ui;
};

#endif // QUERYHISTORYWIDGET_H
