#ifndef SAVEDQUERYWIDGET_H
#define SAVEDQUERYWIDGET_H

#include <QWidget>

namespace Ui {
class SavedQueryWidget;
}

class SavedQueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SavedQueryWidget(QWidget *parent = 0);
    ~SavedQueryWidget();

    void addQuery(QString query);

private:
    Ui::SavedQueryWidget *ui;

    QString getName();

signals:
    void addButtonClicked();

private slots:
    void on_addButton_clicked();
};

#endif // SAVEDQUERYWIDGET_H
