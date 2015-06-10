#include "SavedQueryWidget.h"
#include "ui_SavedQueryWidget.h"

#include <QDebug>

SavedQueryWidget::SavedQueryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SavedQueryWidget)
{
    ui->setupUi(this);
}

SavedQueryWidget::~SavedQueryWidget()
{
    delete ui;
}

void SavedQueryWidget::addQuery(QString query)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::DisplayRole, query);
    //item->setData(Qt::DisplayRole, "Title");
    //item->setData(Qt::UserRole, query);
    ui->listWidget->addItem(item);
    //http://stackoverflow.com/questions/6905147/qt-qlistwidgetitem-multiple-lines
}

void SavedQueryWidget::on_addButton_clicked()
{
    emit addButtonClicked();
}
