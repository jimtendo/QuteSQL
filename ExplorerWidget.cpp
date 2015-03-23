#include "ExplorerWidget.h"
#include "ui_ExplorerWidget.h"

#include <QListWidgetItem>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

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

void ExplorerWidget::on_removeButton_clicked()
{
    // Display messagebox for confirmation
    if (QMessageBox::Yes == QMessageBox::question(this, "Drop Table", "Are you sure you want to drop this table?")) {

        // Get table name
        QString table = ui->tableListWidget->currentItem()->text();

        // Run the drop query
        QSqlQuery query = m_database->exec("DROP TABLE " + table);

        // Check if there was an error and display it if there was
        if (query.lastError().type() != QSqlError::NoError) {
            QMessageBox::critical(this, "Operation failed", query.lastError().text());
        }

        // Refresh explorer
        refresh();
    }
}
