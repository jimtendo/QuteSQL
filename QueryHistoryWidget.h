#ifndef QUERYHISTORYWIDGET_H
#define QUERYHISTORYWIDGET_H

#include <QWidget>

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

private:
    Ui::QueryHistoryWidget *ui;
};

#endif // QUERYHISTORYWIDGET_H
