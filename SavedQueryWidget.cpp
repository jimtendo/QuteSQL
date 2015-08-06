#include "SavedQueryWidget.h"
#include "ui_SavedQueryWidget.h"

#include <QDebug>
#include <QSettings>

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

void SavedQueryWidget::setDatabase(QSqlDatabase *database)
{
    m_database = database;

    loadQueries();
}

void SavedQueryWidget::addQuery(QString query)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::DisplayRole, query);
    //item->setData(Qt::DisplayRole, "Title");
    //item->setData(Qt::UserRole, query);
    ui->listWidget->addItem(item);
    //http://stackoverflow.com/questions/6905147/qt-qlistwidgetitem-multiple-lines

    saveQueries();
}

void SavedQueryWidget::on_addButton_clicked()
{
    emit addButtonClicked();
}

void SavedQueryWidget::on_removeButton_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete item;

    saveQueries();
}

void SavedQueryWidget::loadQueries()
{
    // Load saved connections
    QSettings settings;
    int size = settings.beginReadArray(m_database->connectionName());
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);

        // Add item to combo box
        ui->listWidget->addItem(settings.value("query").toString());
    }
    settings.endArray();
}

void SavedQueryWidget::saveQueries()
{
    // Save queries
    QSettings settings;
    settings.beginWriteArray(m_database->connectionName());
    for (int i = 0; i < ui->listWidget->count(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("query", ui->listWidget->item(i)->text());
    }
    settings.endArray();
}
