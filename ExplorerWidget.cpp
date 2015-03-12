#include "ExplorerWidget.h"
#include "ui_ExplorerWidget.h"

#include <QListWidgetItem>

ExplorerWidget::ExplorerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerWidget)
{
    ui->setupUi(this);
}

ExplorerWidget::~ExplorerWidget()
{
    delete ui;
}

void ExplorerWidget::setDatabase(QSqlDatabase *database)
{
    // Set the database
    m_database = database;

    // Set the database for child widgets
    ui->browseTab->setDatabase(m_database);
    ui->schemaTab->setDatabase(m_database);

    // Refresh table list
    refresh();
}

void ExplorerWidget::refresh()
{
    // Clear list of tables
    ui->tableListWidget->clear();

    // Repopulate list of tables
    foreach (QString table, m_database->tables()) {

        QListWidgetItem *tableItem = new QListWidgetItem(QIcon::fromTheme("folder"), table);
        ui->tableListWidget->addItem(tableItem);
    }
}

void ExplorerWidget::on_tableListWidget_itemActivated(QListWidgetItem *item)
{
    ui->browseTab->setTable(item->text());
    ui->schemaTab->setTable(item->text());
}
