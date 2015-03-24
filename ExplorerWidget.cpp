#include "ExplorerWidget.h"
#include "ui_ExplorerWidget.h"

#include <QListWidgetItem>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>

ExplorerWidget::ExplorerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerWidget),
    m_extension(NULL)
{
    ui->setupUi(this);
}

ExplorerWidget::~ExplorerWidget()
{
    delete ui;
}

void ExplorerWidget::init(QSqlDatabase *database, Extension *extension)
{
    // Set the database
    m_database = database;

    // Set the extension
    m_extension = extension;

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

        // Drop table if extension supports it
        if (m_extension) {
            m_extension->removeTable(table);
        }

        else {
            QMessageBox::warning(this, "Sorry", "This functionality is not available for this database type yet");
        }

        // Refresh explorer
        refresh();
    }
}

void ExplorerWidget::on_addButton_clicked()
{
    // Get table name
    QString table = QInputDialog::getText(this, "Create Table", "Enter table name");

    // Drop table if extension supports it
    if (m_extension) {
        m_extension->createTable(table);
    }

    else {
        QMessageBox::warning(this, "Sorry", "This functionality is not available for this database type yet");
    }

    // Refresh explorer
    refresh();
}
