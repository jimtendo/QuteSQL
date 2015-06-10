#include "QueryHistoryWidget.h"
#include "ui_QueryHistoryWidget.h"

QueryHistoryWidget::QueryHistoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryHistoryWidget)
{
    ui->setupUi(this);
}

QueryHistoryWidget::~QueryHistoryWidget()
{
    delete ui;
}

void QueryHistoryWidget::addQuery(QString query)
{
    ui->commandListWidget->addItem(query);
}
